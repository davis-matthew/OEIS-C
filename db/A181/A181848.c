/*
 * A181848  Consider two consecutive primes {p,q} such that P=2p+q and Q=2q+p
 *          are both prime. Sequence gives lesser primes p.
 * https://oeis.org/A181848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A181848 a(n) = the n-th lesser prime p of a consecutive prime
 *            pair * {p,q} such that P = 2p + q and Q = 2q + p are both prime
 * Range:     u8 n <= 8, u16 n <= 172, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Static data used for memoisation across calls. */
static idx_t          prevPrime       = 2;         /* previous prime in the scan   */
static idx_t          candidate       = 3;         /* candidate for next prime     */
static bool           initialized     = false;      /* first‑time initialisation    */
static seq_t        *results         = NULL;       /* array of found p's           */
static size_t        res_cap         = 0;          /* capacity of results array    */
static size_t        res_cnt         = 0;          /* number of valid results      */
static idx_t         max_needed      = 0;          /* highest n requested so far   */

/* Ensure results array has space for at least 'need' elements. */
static void ensure_res_capacity(size_t need) {
    if (need <= res_cap) return;
    size_t new_cap = res_cap == 0 ? 16 : res_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* memory failure */
    results   = tmp;
    res_cap   = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* OFFSET isn’t used but guard against 0 */

    /* Initialise static state on first call. */
    if (!initialized) {
        initialized = true;
        prevPrime   = 2;
        candidate   = 3;
        res_cap     = 0;
        results     = NULL;
        res_cnt     = 0;
        max_needed  = 0;
    }

    /* Generate sufficient results if we haven’t already. */
    if (n > max_needed) {
        max_needed = n;
        ensure_res_capacity(max_needed);

        while (res_cnt < max_needed) {
            /* Find the next prime q (candidate is always odd). */
            while (!is_prime(candidate)) {
                candidate += 2;
            }
            idx_t q = candidate;
            candidate = q + 2;            /* next odd after q           */

            /* Check the pair {prevPrime, q}. */
            idx_t p = prevPrime;
            idx_t P = 2 * p + q;
            idx_t Q = 2 * q + p;
            if (is_prime(P) && is_prime(Q)) {
                results[res_cnt++] = (seq_t)p;
            }

            /* Advance the sliding window. */
            prevPrime = q;
        }
    }

    return results[n - 1]; /* 0‑based indexing in the results array */
}
