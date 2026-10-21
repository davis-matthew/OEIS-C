/*
 * A222258  Lexicographically earliest injective sequence of nonnegative
 *          integers such that the sum of 8 consecutive terms is always
 *          divisible by 8.
 * https://oeis.org/A222258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          if n % 4 != 3                                  */
/*          else a(n) = 2*n - 3 (i.e. 2*n - 3 when n ≡ 3 (mod 4)) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Promote to seq_t first to avoid overflow in the multiplication
       for implementations where seq_t may be larger than idx_t. */
    seq_t N = (seq_t)n;
    if ((n & 3ULL) == 3ULL) {
        return N * 2 - 3;
    } else {
        return N;
    }
}
