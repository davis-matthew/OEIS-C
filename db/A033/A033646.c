/*
 * A033646  Base 9 digital convolution sequence.
 * https://oeis.org/A033646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0)=1, for n>0 a(n) = Σ_{i=0}^{k-1} d_i * a(n-1-i) where the
 *            d_i are the digits of n in base‑9 (least significant first)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* this file must compile as C11 and C++17 */

#include <stdlib.h>
#include <stdint.h>

/* external types defined by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static size_t cap = 0;               /* number of allocated entries */

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        /* realloc with explicit cast for C++. */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* compute new entries */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            if (i == 0) {
                cache[(size_t)i] = 1;   /* a(0) */
            } else {
                seq_t sum = 0;
                idx_t t = i;
                size_t pos = 0;
                /* iterate over base‑9 digits (least significant first) */
                while (t > 0) {
                    idx_t digit = t % 9;
                    if (digit != 0) {
                        seq_t prod = cache[(size_t)(i - 1 - pos)] * (seq_t)digit;
                        sum += prod;
                    }
                    t /= 9;
                    ++pos;
                }
                cache[(size_t)i] = sum;
            }
        }
        cap = new_cap;
    }
    return cache[(size_t)n];
}
