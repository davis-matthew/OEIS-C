/*
 * A061779  Primes p such that q-p = 22, where q is the next prime after p.
 * https://oeis.org/A061779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 145, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that the next prime after p differs by 22
   (OEIS A061779).  Index is 1‑based: a(1) = 1129.  The routine caches
   previously found terms so that calls in arbitrary order are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrime.h"
#include "../utils/next_prime.h"

static seq_t *grow_seq(seq_t *arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        /* cast the result of realloc to seq_t*  (required by the rules) */
        seq_t *tmp = (seq_t *)realloc(arr, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failure */ exit(1); }
        arr = tmp;
        *cap = newcap;
    }
    return arr;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for 0, return 0 */

    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static idx_t last_checked = 2;   /* next candidate prime to test */

    while (terms_len < (size_t)n) {
        idx_t p;
        /* find the next prime >= last_checked */
        if (isPrime(last_checked)) {
            p = last_checked;
        } else {
            p = next_prime((uint64_t)last_checked);
        }

        idx_t q = next_prime((uint64_t)(p + 1));   /* prime after p */

        if ((q - p) == 22ULL) {
            terms = grow_seq(terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = (seq_t)p;
        }

        /* continue searching after p */
        last_checked = p + 1;
    }

    return terms[n - 1];
}
