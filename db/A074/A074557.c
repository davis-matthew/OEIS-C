/*
 * A074557  a(n) = 3^n + 6^n + 9^n.
 * https://oeis.org/A074557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 6^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n, 6^n and 9^n separately using simple repeated
       multiplication; this guarantees correctness for all n in the
       64‑bit build (where the result might overflow but the overflow
       is well defined for unsigned types) and works for arbitrary‑precision
       seq_t in the big build. */
    seq_t p3 = 1;
    seq_t p6 = 1;
    seq_t p9 = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        p3 *= 3;
        p6 *= 6;
        p9 *= 9;
    }
    return p3 + p6 + p9;
}
