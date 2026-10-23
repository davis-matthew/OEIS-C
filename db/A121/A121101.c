/*
 * A121101  Catapolyoctagons (see Cyvin et al. for precise definition).
 * https://oeis.org/A121101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*(1 + 5^(n-2) + 2*(2-(-1)^n)*5^(floor(n/2)-1)) for
 *            n>=2; a(1)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t pow_n2 = power5(n - 2);          /* 5^(n-2) */
    seq_t pow_half = power5((n / 2) - 1);   /* 5^(floor(n/2)-1) */

    seq_t temp;
    if (n & 1) /* n odd */
        temp = (seq_t)1 + pow_n2 + pow_half * (seq_t)6;
    else       /* n even */
        temp = (seq_t)1 + pow_n2 + pow_half * (seq_t)2;

    return temp / (seq_t)4;
}
