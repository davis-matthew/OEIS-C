/*
 * A066024  Numbers k such that the product of the digits of k minus the sum
 *          of the digits of k is prime.
 * https://oeis.org/A066024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A066024: Numbers k such that (product of the digits of k) - (sum of the digits of k) is prime.
   The function builds a cache of terms and returns the nth term (1‑indexed). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* Cached terms: cache[i] = a(i+1) */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_checked = 0;   /* largest integer examined so far */

    /* Keep filling the cache until it has at least n terms. */
    while (cache_len < n) {
        idx_t k = ++last_checked;
        seq_t prod = 1;
        seq_t sum  = 0;
        idx_t t    = k;
        bool ok = true;

        /* Process digits of k. */
        while (t > 0) {
            unsigned digit = t % 10;
            t /= 10;
            if (digit == 0) {            /* product becomes 0 → diff ≤ 0 */
                ok = false;
                break;
            }
            prod *= (seq_t)digit;
            sum  += (seq_t)digit;
        }

        if (ok) {
            seq_t diff = prod - sum;
            if (diff != (seq_t)0 && isPrimeTrial(diff)) {
                /* Append the term to the cache. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
                    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!p) abort();   /* allocation failure – terminate */
                    cache = p;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)k;
            }
        }
    }

    return cache[n - 1];
}
