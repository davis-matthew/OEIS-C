/*
 * A098610  a(n) = 10^n + (-1)^n.
 * https://oeis.org/A098610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n + (-1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 – a(0) is the first term */

/* Computes the n-th term of OEIS A098610. */
seq_t a(idx_t n)
{
    seq_t val = 1;              /* start with 10^0 = 1 */
    for (idx_t i = 0; i < n; ++i)
        val *= (seq_t)10;      /* multiply by 10 n times */

    if (n & 1U)                /* odd n : subtract 1  */
        val -= (seq_t)1;
    else                       /* even n : add 1  */
        val += (seq_t)1;

    return val;
}
