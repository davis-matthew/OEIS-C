/*
 * A094002  a(n+3) = 3*a(n+2) - 2*a(n+1) + 1 with a(0)=1, a(1)=5.
 * https://oeis.org/A094002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n - n - 4 (Recurrence: a(n+3) = 3*a(n+2) - 2*a(n+1) +
 *            1, a(0)=1, a(1)=5)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n in seq_t */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;      /* multiply by 2 */

    /* a(n) = 5*2^n - n - 4  */
    seq_t result = pow2 * (seq_t)5;   /* 5*2^n   */
    result -= (seq_t)n;                /* minus n  */
    result -= (seq_t)4;                /* minus 4  */
    return result;
}
