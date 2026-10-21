/*
 * A074017  Number of elements of GF(7^n) with trace 1 and subtrace 0.
 * https://oeis.org/A074017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 7^(n-2) + c(n)*7^⌊(n-2)/2⌋ where for n≥3 k = ⌊(n-2)/2⌋, r
 *            = k mod 3, first = (n-2) even (i.e., n = 2k+2). Then if r==0: c
 *            = first?0:-1 if r==1: c = first?-1:1 if r==2: c = first?1:0
 *            Special cases: a(1)=1, a(2)=0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)0;

    idx_t k = (n - 2) / 2;                     /* ⌊(n-2)/2⌋ */
    idx_t r = k % 3;                           /* k modulo 3 */
    int first = ((n - 2) & 1ULL) == 0;         /* n = 2k+2 ? */

    int coeff;
    if (r == 0) {
        coeff = first ? 0 : -1;
    } else if (r == 1) {
        coeff = first ? -1 : 1;
    } else { /* r == 2 */
        coeff = first ? 1 : 0;
    }

    seq_t base = big_pow((seq_t)7, (seq_t)(n - 2)); /* 7^(n-2) */
    if (coeff == 0)
        return base;

    seq_t extra = big_pow((seq_t)7, (seq_t)k);     /* 7^k */

    if (coeff == 1)
        return base + extra;
    else /* coeff == -1 */
        return base - extra;
}
