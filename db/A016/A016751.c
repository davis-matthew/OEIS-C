/*
 * A016751  a(n) = (2*n)^11.
 * https://oeis.org/A016751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^11
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 28, u128 n <= 1591,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (2*n)^11 by repeated multiplication.
       This works for any seq_t, including arbitrary‑precision types. */
    seq_t base = (seq_t)2 * (seq_t)n;
    seq_t res  = 1;
    for (int i = 0; i < 11; ++i)
        res *= base;
    return res;
}
