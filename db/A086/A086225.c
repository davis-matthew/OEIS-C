/*
 * A086225  a(n) = 11*2^n - 1.
 * https://oeis.org/A086225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*2^n - 1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n in a loop. For arbitrary‑precision seq_t this will
       work correctly in all builds. For the built‑in unsigned integer
       types the shift by 1 is defined as long as the shift amount (<width)
       is less than the type width. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;          /* multiply by 2 */

    return (pow2 * (seq_t)11) - (seq_t)1;
}
