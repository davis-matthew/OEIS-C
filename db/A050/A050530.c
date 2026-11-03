/*
 * A050530  Numbers k such that k - phi(k) is prime.
 * https://oeis.org/A050530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4845, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9282 (2026-11-03)
 * Helpers:   is_prime, mem, totient
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
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

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
        seq_t phi_k = totient((idx_t)k);      /* φ(k) */
        seq_t diff  = k - phi_k;          /* k - φ(k) */

        if (is_prime((idx_t)diff)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
