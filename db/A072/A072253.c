/*
 * A072253  Numbers n for which one step of the Collatz iteration (3n+1)/2^r
 *          gives rise to values 59,53,47,41,35,29,23,17,11 and 5 for
 *          r=1,3,5,..,19.
 * https://oeis.org/A072253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = ((118 - 12*(n-1))*4^(n-1) - 1) / 3
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0 */

    /* k = n-1;  compute 4^k */
    idx_t k = n - 1;
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow4 *= (seq_t)4;

    /* coefficient: 118 - 12*k */
    seq_t coeff = (seq_t)118 - (seq_t)12 * (seq_t)k;

    /* numerator: coeff * pow4 - 1 */
    seq_t num = coeff * pow4;
    num -= (seq_t)1;

    /* divide by 3 (exact division) */
    return num / (seq_t)3;
}
