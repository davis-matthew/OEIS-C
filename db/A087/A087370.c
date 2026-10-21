/*
 * A087370  Numbers n such that 3n - 1 is a prime.
 * https://oeis.org/A087370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th positive integer k such that 3*k−1 is prime
 * Range:     u8 n <= 69, u16 n <= 8856, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset 1 */

    idx_t count = 0;                /* how many qualifying integers found */
    idx_t k     = 0;                /* current candidate integer */

    for (;;) {
        ++k;                        /* test next integer */
        idx_t p = 3 * k - 1;        /* 3*k−1 to be tested for primality */

        if (is_prime(p)) {
            ++count;                /* another qualifying integer */
            if (count == n) {
                return (seq_t)k;    /* a(n) */
            }
        }
    }
}
