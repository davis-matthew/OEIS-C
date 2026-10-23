/*
 * A163141  a(n) = 5*a(n-2) for n > 2; a(1) = 4, a(2) = 5.
 * https://oeis.org/A163141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2), a(1)=4, a(2)=5
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;

    if (n & 1U) {                       /* odd index */
        seq_t k = (seq_t)((n - 1) / 2);
        seq_t pow5 = big_pow((seq_t)5, k);
        return 4 * pow5;
    } else {                            /* even index */
        seq_t k = (seq_t)(n / 2);
        return big_pow((seq_t)5, k);
    }
}
