/*
 * A199566  a(n) = (7*9^n + 1)/2.
 * https://oeis.org/A199566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 9*a(n-1) - 4, a(0) = 4 */
/* Equivalent formula: a(n) = (7*9^n + 1)/2 */
seq_t a(idx_t n)
{
    /* Fast recurrence; works for any seq_t (including big integers) */
    if (n == 0)
        return (seq_t)4;

    seq_t val = (seq_t)4;              /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        val = val * (seq_t)9 - (seq_t)4;

    return val;
}
