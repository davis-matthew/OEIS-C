/*
 * A060870  Number of n X n matrices over GF(5) with rank 1.
 * https://oeis.org/A060870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5^n - 1)^2) / 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t five = (seq_t)5;
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow5 *= five;                /* 5^n */

    seq_t sub  = pow5 - (seq_t)1;      /* 5^n - 1  (even) */
    seq_t half = sub / (seq_t)2;       /* (5^n - 1)/2 */
    return half * half;                /* the desired value */
}
