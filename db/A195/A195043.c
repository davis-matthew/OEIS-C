/*
 * A195043  Concentric 11-gonal numbers.
 * https://oeis.org/A195043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n^2 + 7*((-1)^n - 1)) / 8
 * Range:     u8 n <= 3, u16 n <= 153, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to the OEIS definition: a(n) = 11*n^2/4 + 7*((-1)^n - 1)/8 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* index cast to seq_t for arithmetic */
    seq_t num = (seq_t)22 * nn * nn;   /* 22 * n^2 */
    if (n & 1)                       /* n odd => (-1)^n = -1 */
        num -= (seq_t)14;              /* subtract 7*2 = 14 */
    return num / (seq_t)8;            /* divide by 8 */
}
