/*
 * A097813  a(n) = 3*2^n - 2*n - 2.
 * https://oeis.org/A097813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*2^n - 2*n - 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n by repeated left shifts.
       This works for all seq_t types, including arbitrary‑precision.    */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;

    /* a(n) = 3*2^n - 2*n - 2 */
    seq_t result = (seq_t)3 * pow2;
    result -= (seq_t)2 * ((seq_t)n);
    result -= (seq_t)2;
    return result;
}
