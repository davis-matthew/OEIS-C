/*
 * A051040  5-Stohr sequence.
 * https://oeis.org/A051040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) for 1 ≤ n ≤ 6, * a(n) = 31*n - 154 for n ≥ 7 *
 *            Recurrence (following the linear form): a(n) = 2*a(n-1) - a(n-2)
 *            for n > 7
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6)
        return ((seq_t)1) << (n - 1);
    else
        return (seq_t)31 * (seq_t)n - (seq_t)154;
}
