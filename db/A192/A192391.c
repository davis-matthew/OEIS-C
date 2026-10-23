/*
 * A192391  Numbers k such that the number of primes in the open interval (k^2
 *          - k, k^2) is equal to the number of primes in the open interval
 *          (k^2, k^2 + k).
 * https://oeis.org/A192391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n-th integer k such that the number of primes in the
 * open interval (k²–k, k²) equals the number of primes in (k², k²+k).
 *
 * The implementation incrementally tests successive integers k, using
 * compute_pi to obtain prime counts quickly, and memoises found terms.
 * Calls may arrive in any order.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/compute_pi.h"

/* memoisation storage */
static seq_t *cached = NULL;      /* array of found k's */
static size_t cap = 0;            /* capacity of cached array */
static size_t len = 0;            /* number of stored values */
static idx_t next_k = 1;          /* next k to test */

/* enlarge the cache array when needed */
static void grow_cache(void)
{
    size_t newcap = cap ? cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(cached, newcap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */
    cached = tmp;
    cap = newcap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* fetch until we have at least n terms */
    while (len < n) {
        idx_t k = next_k++;                       /* test next integer k */
        idx_t k2 = k * k;                         /* k² */

        /* counts of primes in the two open intervals */
        seq_t left  = compute_pi(k2 - 1) - compute_pi(k2 - k);
        seq_t right = compute_pi(k2 + k - 1) - compute_pi(k2);

        if (left == right) {                      /* k satisfies the condition */
            if (len == cap) grow_cache();
            cached[len++] = (seq_t)k;             /* store it */
        }
    }
    return cached[n - 1];
}
