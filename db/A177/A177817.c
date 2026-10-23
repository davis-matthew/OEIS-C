/*
 * A177817  Numbers k such that k^3 divides 17^(k^2) + 1.
 * https://oeis.org/A177817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k^3 | 17^(k^2)+1 */
#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /*------------------------------------------------------------
     *  Lazy memoization of the sequence terms.
     *  cache[i] holds a(i+1).  last_k is the largest k already tested.
     *------------------------------------------------------------*/
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t last_k = 0;

    if (n == 0) return 0;   /* not used, offset is 1 */

    /* grow cache as needed */
    if (cap == 0) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while (len < (size_t)n) {
        /* next candidate k */
        last_k++;

        /* 17 must be coprime to k; skip multiples of 17 */
        if ((last_k % 17) == 0) continue;

        /* compute modulus m = k^3 */
        seq_t m = last_k * last_k * last_k;

        /* exponent e = k^2 */
        seq_t e = last_k * last_k;
        /* cast to idx_t (unsigned 64‑bit) – it fits for all tested n */
        idx_t exp = (idx_t)e;

        /* 17^e mod m */
        seq_t val = mod_pow(17, exp, m);

        /* check if val == m-1   (<=> 17^e + 1 ≡ 0 (mod m)) */
        if (val == (m - 1)) {
            if (len == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = last_k;
        }
    }

    return cache[n - 1];
}
