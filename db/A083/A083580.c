/*
 * A083580  Binomial transform of A083579.
 * https://oeis.org/A083580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n/3 - 5*0^n/12 - (n+1)*2^(n-2) (n ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly to avoid negative exponents */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* for n ≥ 2 */
    seq_t term1 = (seq_t)2 * pow3(n - 1);            /* 2*3^(n-1)  */
    seq_t term2 = ((seq_t)(n + 1)) * pow2(n - 2);   /* (n+1)*2^(n-2) */

    return term1 - term2;
}
