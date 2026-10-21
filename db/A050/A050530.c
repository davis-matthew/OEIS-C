/*
 * A050530  Numbers k such that k - phi(k) is prime.
 * https://oeis.org/A050530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4845, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050530: numbers k such that k - φ(k) is prime.
   The function tests successive integers, computes φ(k) using the
   helper `_phi_value` provided via "../utils/phi.h", checks primality
   of the difference with `is_prime` from "../utils/is_prime.h",
   and stores accepted terms in a static cache for fast random access. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"      /* provides static seq_t phi(idx_t m) */
#include "../utils/is_prime.h" /* provides bool is_prime(idx_t n) */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static idx_t  next_k    = 1;   /* next integer to test */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t k = (seq_t)next_k++;
        seq_t phi_k = phi((idx_t)k);      /* φ(k) */
        seq_t diff  = k - phi_k;          /* k - φ(k) */

        if (is_prime((idx_t)diff)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
