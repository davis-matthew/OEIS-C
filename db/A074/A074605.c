/*
 * A074605  a(n) = 3^n + 4^n.
 * https://oeis.org/A074605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = 3^n + 4^n  (OEIS A074605) */
/* Computes a(n) for any n ≥ 0.  Handles arbitrary‑precision seq_t and
   unsigned integer seq_t of 8/16/32/64/128 bits.  For builtin integer
   types the simple iterative multiplication is safe whenever the
   final result fits in seq_t, which is the case for all tests. */

seq_t a(idx_t n)
{
    seq_t p3 = 1;
    seq_t p4 = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        p3 *= (seq_t)3;
        p4 *= (seq_t)4;
    }
    return p3 + p4;
}
