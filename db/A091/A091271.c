/*
 * A091271  Numbers k such that 4*k^2-11 is a prime.
 * https://oeis.org/A091271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that 4*k^2-11 is prime
 * Range:     u8 n <= 65, u16 n <= 7054, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *vals = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t next_k = 2;   /* next integer k to test */

    if (n == 0) return 0;  /* undefined for offset 0 */

    /* produce terms until we have n of them */
    while (len < n) {
        /* compute 4*k^2-11 in 64‑bit arithmetic (fits for all n tested) */
        uint64_t v = 4ULL * (uint64_t)next_k * (uint64_t)next_k - 11ULL;

        if (is_prime((idx_t)v)) {
            /* grow array if necessary */
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(vals, newcap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                vals = tmp;
                cap = newcap;
            }
            vals[len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return vals[n - 1];
}
