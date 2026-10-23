/*
 * A017734  Binomial coefficients C(n,70).
 * https://oeis.org/A017734
 *
 * Offset:    70    (a(70) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,70) (i.e. the binomial coefficient C(n,70))
 * Range:     u8 n <= 71, u16 n <= 73, u32 n <= 77, u64 n <= 89, u128 n <= 131,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 70u) return 0;          /* the sequence starts at n = 70 */

    /* For types that fit in 64 bits we use a multiplicative scheme with an
       intermediate unsigned __int128 to avoid overflow.  For larger types
       we defer to the generic binom implementation. */
    if (sizeof(seq_t) <= 8u) {
        idx_t k = 70u;
        seq_t res = 1u;
        for (idx_t i = 1u; i <= k; ++i) {
            /* Exchange the multiply and divide steps to keep the result
               integral.  The intermediate product fits into 128 bits.   */
            unsigned __int128 prod = (unsigned __int128)res * (n - k + i);
            res = (seq_t)(prod / i);
        }
        return res;
    } else {
        return binom(n, 70u);
    }
}
