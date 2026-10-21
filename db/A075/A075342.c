/*
 * A075342  a(1) = 1, a(n+1) is the smallest number such that there are n
 *          primes between a(n) and a(n+1) exclusive.
 * https://oeis.org/A075342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   estimate_limit_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A075342
 *
 * Definition: a(1) = 1.
 * For n ≥ 1, a(n+1) is the smallest integer m > a(n) such that
 * there are exactly n prime numbers strictly between a(n) and m.
 *
 * Implementation notes:
 *   • A cached sieve of Eratosthenes is maintained up to a bound
 *     that safely covers the required values for all n requested.
 *   • The sequence values are computed incrementally and stored
 *     in a 1‑indexed cache.  Calls to a() may arrive in any order.
 *   • seq_t and idx_t are supplied by the build environment; the
 *     code uses only operators +, -, *, /, %, comparisons and the
 *     logical operator !, and does not use floating-point for
 *     sequence values.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/estimate_limit_for_n.h"

/* ------------------------------------------------------------------ */
/* Prime sieve (bitset over odd numbers)                              */
/* ------------------------------------------------------------------ */
static size_t sieve_limit = 0;      /* highest number sieved           */
static uint8_t *sieve_bits = NULL;  /* bitset: bit=1 ⇒ composite     */

/* Return true if x is marked composite (bit set). Assumes x ≤ sieve_limit. */
static inline int is_composite_bit(seq_t x)
{
    /* cast to UI64 for indexing; safe because x ≤ sieve_limit */
    uint64_t ux = (uint64_t)x;
    if (ux < 3) return 0;                /* 0,1,2 are not composite     */
    if ((ux & 1ULL) == 0) return 1;       /* even numbers >2 are composite*/
    /* map odd number to bit index */
    uint64_t idx = ux >> 1;            /* index of odd number          */
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1U;
}

/* Return true if x is prime. Uses the bitset for x ≤ sieve_limit.     */
static inline int is_prime(seq_t x)
{
    if (x < (seq_t)2) return 0;
    if (x == (seq_t)2) return 1;
    return !is_composite_bit(x);
}

/* ------------------------------------------------------------------ */
/* Build a sieve up to 'limit'                                         */
/* ------------------------------------------------------------------ */
static void rebuild_sieve(size_t limit)
{
    size_t odd_count  = (limit >> 1) + 1;          /* odds ≤ limit          */
    size_t byte_count = (odd_count + 7) >> 3;      /* bytes needed          */

    uint8_t *new_bits = (uint8_t *)calloc(byte_count, 1);
    if (!new_bits) exit(1);                       /* out of memory         */

    if (sieve_bits) free(sieve_bits);
    sieve_bits  = new_bits;
    sieve_limit = limit;

    /* Eratosthenes over odd numbers */
    size_t sqrt_limit = (size_t)sqrt((long double)limit);
    for (size_t i = 3; i <= sqrt_limit; i += 2) {
        if (is_composite_bit((seq_t)i)) continue;
        size_t step = i << 1;                     /* 2 * i */
        size_t start = i * i;                     /* i^2   */
        if (start > limit) break;
        for (size_t j = start; j <= limit; j += step) {
            size_t idx = j >> 1;
            sieve_bits[idx >> 3] |= 1U << (idx & 7);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Very generous upper bound for a(n).  For n ≤ 10 000 this keeps us
 * safely within an 32‑bit limit.                                     */

/* ------------------------------------------------------------------ */
/* Sequence cache (1‑indexed).  seq_cache[1] == a(1) == 1           */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;     /* points to array of seq_t          */
static idx_t  max_cache_n = 0;      /* highest n whose term is known    */

/* Extend cache and sieve as needed to compute up to index n.          */
static void ensure_computed_upto(idx_t n)
{
    if (n <= max_cache_n) return;     /* already computed                 */

    /* Make sure we have a sieve large enough */
    size_t limit_needed = estimate_limit_for_n(n);
    if (limit_needed > sieve_limit) rebuild_sieve(limit_needed);

    /* Grow the cache to hold indices up to n */
    if (!seq_cache || (size_t)(max_cache_n + 1) < (size_t)n + 1) {
        size_t new_len = (size_t)n + 1;            /* 1‑indexed */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);                        /* out of memory */
        seq_cache = tmp;
    }

    /* Initialise first term if cache was empty */
    if (max_cache_n == 0) {
        seq_cache[1] = (seq_t)1;
        max_cache_n = 1;
    }

    /* Compute terms from max_cache_n+1 up to n */
    seq_t cur  = seq_cache[max_cache_n];      /* a(max_cache_n) */
    seq_t cand = cur + (seq_t)1;               /* candidate integer */

    for (idx_t i = max_cache_n; i < n; ++i) { /* i = current index of a(i)   */
        size_t need  = (size_t)i;             /* n primes after a(i)          */
        size_t found = 0;
        seq_t last_prime = 0;

        while (found < need) {
            if (is_prime(cand)) {
                ++found;
                last_prime = cand;
            }
            ++cand;
        }
        seq_t next = last_prime + (seq_t)1;    /* a(i+1) */
        seq_cache[i + 1] = next;
        cur = next;                           /* advance base for next step   */
        cand = cur + (seq_t)1;
    }

    max_cache_n = n;
}

/* ------------------------------------------------------------------ */
/* Public function                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1         */
    ensure_computed_upto(n);
    return seq_cache[n];
}
