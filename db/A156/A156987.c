/*
 * A156987  Primes using all the prime digits 2, 3, 5, 7 repeated in strings,
 *          in that order.
 * https://oeis.org/A156987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the n-th prime composed only of the digits 2,3,5,7
   in strictly ascending order (digits repeat but never decrease). */

#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    /* iterate over lengths of the number (minimum 4 digits) */
    for (idx_t len = 4; ; ++len) {
        /* k2: number of leading '2's, must be at least 1 */
        for (int k2 = (int)(len - 3); k2 >= 1; --k2) {
            idx_t k2u = (idx_t)k2;
            idx_t rem2 = len - k2u;          /* remaining digits after '2' part */

            /* k3: number of '3's, at least 1 */
            for (int k3 = (int)(rem2 - 2); k3 >= 1; --k3) {
                idx_t k3u = (idx_t)k3;
                idx_t rem3 = rem2 - k3u;     /* remaining after '3's */

                /* k5: number of '5's, at least 1 */
                for (int k5 = (int)(rem3 - 1); k5 >= 1; --k5) {
                    idx_t k5u = (idx_t)k5;
                    idx_t k7u = rem3 - k5u;   /* remaining digits are '7's */

                    /* compose the decimal string */
                    char buf[512];
                    char *p = buf;
                    for (idx_t i = 0; i < k2u; ++i) *p++ = '2';
                    for (idx_t i = 0; i < k3u; ++i) *p++ = '3';
                    for (idx_t i = 0; i < k5u; ++i) *p++ = '5';
                    for (idx_t i = 0; i < k7u; ++i) *p++ = '7';
                    *p = '\0';

                    seq_t num = seq_from_str(buf);
                    if (is_prime_big(num)) {
                        ++found;
                        if (found == n) {
                            return num;
                        }
                    }
                }
            }
        }
    }
}
