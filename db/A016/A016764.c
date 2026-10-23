/*
 * A016764  a(n) = (2*n+1)^12.
 * https://oeis.org/A016764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 19, u128 n <= 812,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t, compute the base 2*n+1, then raise to 12. */
    seq_t base   = ((seq_t)n * (seq_t)2) + (seq_t)1;   /* (2*n + 1) */
    seq_t result = (seq_t)1;                           /* 1^(12) */

    for (int i = 0; i < 12; ++i)
        result = result * base;                        /* multiply 12 times */

    return result;
}
