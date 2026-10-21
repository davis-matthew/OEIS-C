/*
 * A066997  Survivor number for 2nd-order Josephus problem.
 * https://oeis.org/A066997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (k < 2^(m-1)) ? 1 + k + 2^(m-1) : 2^m, where m =
 *            floor(log2(n)) and k = n - 2^m. (OEIS A066997)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at n = 2 */

    /* compute m = floor(log2(n)) using bit operations */
    idx_t temp = n, m = 0;
    while (temp >>= 1) ++m;       /* after loop, m = floor(log2(n)) */

    /* 2^m as seq_t */
    seq_t p  = (seq_t)1 << m;      /* p = 2^m           */
    seq_t half = p >> 1;           /* half = 2^(m-1)    */

    /* k = n - 2^m.  k fits in idx_t, cast to seq_t for comparison. */
    idx_t k = n - ((idx_t)1 << m);
    seq_t sk = (seq_t)k;

    /* if k < 2^(m-1) use first branch, else 2^m */
    if (sk < half)
        return (seq_t)1 + sk + half;
    else
        return p;
}
