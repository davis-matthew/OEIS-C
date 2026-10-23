/*
 * A033662  Possible digital sums of Smith numbers (conjectural).
 * https://oeis.org/A033662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 4,9,13,15 for n=1..4; a(n) = n+12 for n>=5
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, fallback */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)15;
    /* For n >= 5, a(n) = n + 12 */
    return (seq_t)n + (seq_t)12;
}
