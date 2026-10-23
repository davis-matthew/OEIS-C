/*
 * A104585  a(n) = (1/2) * ( 3*n^2 - n*(-1)^n ).
 * https://oeis.org/A104585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * (3*n^2 - n*(-1)^n) offset 0
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of A104585 */
seq_t a(idx_t n)
{
    /* Convert the index to the target type for arithmetic */
    seq_t sn = (seq_t)n;
    /* Compute 3*n^2 */
    seq_t sq = sn * sn;          /* n^2   */
    seq_t thn = sq * 3;          /* 3*n^2 */

    /* If n is odd, (-1)^n = -1 so we add n;
       if n is even, (-1)^n =  1 so we subtract n */
    if (n & 1)
        return (thn + sn) / 2;   /* (3*n^2 + n) / 2 */
    else
        return (thn - sn) / 2;   /* (3*n^2 - n) / 2 */
}
