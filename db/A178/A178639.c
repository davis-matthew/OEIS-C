/*
 * A178639  Numbers m such that all three values m^2 + 13^k, k = 1, 2, 3, are
 *          prime.
 * https://oeis.org/A178639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for m such that m^2+13^k (k=1,2,3) are all prime */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined – return 0 */
    idx_t found = 0;
    idx_t m = 0;           /* candidate */

/*  m must be even and last digit 0,2,8 */
    for (;; ++m) {
        if ((m & 1ULL) == 0) {
            uint8_t d = m % 10;
            if (d == 0 || d == 2 || d == 8) {
                seq_t mm = (seq_t)m;
                seq_t sq = mm * mm;
                seq_t v1 = sq + (seq_t)13;     /* 13^1 */
                seq_t v2 = sq + (seq_t)169;    /* 13^2 */
                seq_t v3 = sq + (seq_t)2197;   /* 13^3 */

                if (is_prime_big(v1)
                 && is_prime_big(v2)
                 && is_prime_big(v3)) {
                    ++found;
                    if (found == n) return (seq_t)m;
                }
            }
        }
    }
}
