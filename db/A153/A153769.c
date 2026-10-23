/*
 * A153769  Numbers n such that 8n-9 is not prime.
 * https://oeis.org/A153769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth nonnegative integer m such that 8*m - 9 is not
 *            prime (offset 1, so a(1)=0)
 * Range:     u8 n <= 178, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 0; ; ++m) {
        bool ok;
        if (m < 2) {
            /* For m = 0 and 1, 8*m-9 is negative; treat as not prime. */
            ok = true;
        } else {
            idx_t val = 8 * m;
            val -= 9;
            ok = !is_prime(val);
        }
        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)m;
            }
        }
    }
}
