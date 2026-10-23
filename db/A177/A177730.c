/*
 * A177730  Expansion of (6*x + 1) / ((x - 1)*(2*x - 1)*(4*x - 1)*(8*x - 1)).
 * https://oeis.org/A177730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2^(n+1)-1)^2 * (2^(n+2)-1)) / 3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(n+1) and 2^(n+2) as seq_t */
    seq_t t1 = ((seq_t)1 << (n + 1)) - 1;   /* 2^(n+1) - 1 */
    seq_t t2 = ((seq_t)1 << (n + 2)) - 1;   /* 2^(n+2) - 1 */

    /* numerator: (t1 * t1 * t2) */
    seq_t num = t1 * t1;
    num = num * t2;

    /* divide by 3; division is exact for all n */
    return num / 3;
}
