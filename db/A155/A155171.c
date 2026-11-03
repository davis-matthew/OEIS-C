/*
 * A155171  Numbers p such that if q = p+1 then (a = q^2-p^2, b = 2*p*q, c =
 *          q^2 + p^2) is a primitive Pythagorean triple with s-1 and s+1
 *          primes, where s = a+b+c.
 * https://oeis.org/A155171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function computes A155171: the numbers p for which
 * q = p+1 yields a primitive Pythagorean triple (a,b,c) = (q²‑p², 2pq, q²+p²)
 * such that s−1 and s+1 are prime, where s = a+b+c.
 * In terms of p, the condition is that
 *     s = 4p² + 6p + 2
 * and both s-1 and s+1 are prime.
 * The sequence is generated on demand and cached for efficient repeated
 * calls.  Primality tests are performed with the provided is_prime_big()
 * routine.
 *
 * Since the sequence terms are strictly increasing, the algorithm
 * remembers the last tested p and expands the cache only when a
 * larger index is requested.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n) {
    /* Static cache for already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* capacity of the array */
    static idx_t last_p_checked = 0; /* last p value that was tested */

    /* Expand cache until it contains at least n terms */
    while (cache_len < n) {
        /* Test next p value */
        ++last_p_checked;  /* p to test */
        seq_t p = (seq_t)last_p_checked;

        /* Compute s = 4p² + 6p + 2 in seq_t */
        seq_t s = 4 * p * p + 6 * p + 2;

        /* Check primality of s-1 and s+1 */
        if (is_prime_big(s - 1) && is_prime_big(s + 1)) {
            append_candidate(&cache, &cache_len, &cache_cap, p);
        }
    }

    return cache[n - 1];
}
