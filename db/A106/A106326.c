/*
 * A106326  Smallest odd semiprime not less than n.
 * https://oeis.org/A106326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest odd semiprime ≥ n
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (an odd semiprime is a product of two odd primes,
   e.g. 9 = 3×3, 15 = 3×5, 21 = 3×7, …)               */

#include <stdbool.h>
#include "../utils/is_prime_odd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    seq_t cur = n;
    /* Ensure we start with an odd number.            */
    if ((cur & 1) == 0) cur++;
    /* 9 is the first odd semiprime.                 */
    if (cur < 9) cur = 9;

    for (;;) {
        bool semiprime = false;
        for (seq_t p = 3; p * p <= cur; p += 2) {
            if (cur % p == 0) {
                if (is_prime_odd(p)) {
                    seq_t q = cur / p;
                    if (is_prime_odd(q)) {
                        semiprime = true;
                        break;
                    }
                }
            }
        }
        if (semiprime) return cur;
        cur += 2;               /* next odd candidate */
    }
}
