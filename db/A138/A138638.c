/*
 * A138638  Primes of form 19*n-1.
 * https://oeis.org/A138638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incremental search for primes of the form 19*k-1.
   The sequence is indexed starting at 1 as in OEIS. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;          /* array of a(k) values   */
static size_t cache_size = 0;        /* number of cached terms */
static size_t cache_cap   = 0;        /* current allocation cap   */
static idx_t last_checked_k = 1;     /* last k that was examined */

/* Ensure capacity for at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;

#ifdef __cplusplus
    /* C++: use new[] and delete[] which invoke constructors/destructors. */
    seq_t *newptr = new seq_t[newcap];
    if (cache)
        delete[] cache;
#else
    /* C: use realloc which only handles POD types. */
    seq_t *newptr = realloc(cache, newcap * sizeof(seq_t));
#endif

    cache     = newptr;
    cache_cap = newcap;
}

/* Return the nth term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)              /* out of range; not used in tests */
        return (seq_t)0;

    /* Grow the cache until the nth term is present. */
    while (cache_size < (size_t)n)
    {
        /* Search for the next prime of the form 19*k-1. */
        for (idx_t k = last_checked_k + 1; ; ++k)
        {
            seq_t candidate = (seq_t)k * 19 - 1;
            if (is_prime_big(candidate))
            {
                ensure_cache(cache_size + 1);
                cache[cache_size++] = candidate;
                last_checked_k = k;
                break;          /* found one, go to next term if needed */
            }
        }
    }

    return cache[n - 1];
}
