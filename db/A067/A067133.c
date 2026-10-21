/*
 * A067133  n is a term if the phi(n) numbers in [0,n-1] and coprime to n form
 *          an arithmetic progression.
 * https://oeis.org/A067133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth smallest integer such that it is 1, 6, a prime, or a
 *            power of 2
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t val = 1; ; ++val) {
        /* 1 and 6 are handled explicitly.  Powers of 2 (including 1) are detected by the bit trick. */
        if (val == 6 || ((val & (val - 1)) == 0) || is_prime(val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
}
