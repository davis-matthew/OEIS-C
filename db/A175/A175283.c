/*
 * A175283  Numbers k with the property that k and k^2 + 3k+9 are primes.
 * https://oeis.org/A175283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find all k such that k and k^2 + 3k + 9 are primes.  A(n) is the
 * n‑th such integer (1‑indexed).  The implementation keeps a static
 * cache of the terms found so far, extending it incrementally.
 * Primality is tested with is_prime_big, which works for arbitrary‑
 * precision seq_t values. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

/* Return the n-th term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    /* Static cache of terms found so far. */
    static seq_t *cache   = NULL;   /* array of found k's   */
    static size_t cache_size = 0;   /* how many are stored  */
    static size_t cache_cap  = 0;   /* allocated capacity   */
    static idx_t  next_candidate = 1;/* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Move to the next integer candidate. */
        next_candidate++;

        /* Convert to seq_t for arithmetic. */
        seq_t k = (seq_t)next_candidate;
        if (!is_prime_big(k))
            continue;                     /* candidate not prime    */

        /* Compute s = k^2 + 3k + 9 using seq_t arithmetic. */
        seq_t s = k * k + 3 * k + 9;
        if (!is_prime_big(s))
            continue;                     /* the quadratic not prime */

        /* Grow cache if necessary. */
        if (cache_size == cache_cap)
            ensure_capacity(cache_size + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));

        /* Store the found term. */
        cache[cache_size++] = k;
    }

    return cache[(size_t)n - 1];
}
