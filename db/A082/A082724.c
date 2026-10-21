/*
 * A082724  a(n) = (3*11^n + 3^n)/4.
 * https://oeis.org/A082724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*11^n + 3^n)/4 (offset 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 11^n and 3^n iteratively.  No dynamic allocation
       is required; the routine is O(n) which is fast for n ≤ ~10⁴
       and satisfies the required time constraints. */
    seq_t pow11 = 1;
    seq_t pow3  = 1;

    for (idx_t i = 0; i < n; ++i)
    {
        pow11 *= (seq_t)11;
        pow3  *= (seq_t)3;
    }

    /* The expression is always divisible by 4, as guaranteed by the
       formula; division truncates correctly. */
    return ((seq_t)3 * pow11 + pow3) / 4;
}
