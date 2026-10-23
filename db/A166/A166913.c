/*
 * A166913  a(n) = 20*a(n-1) - 64*a(n-2) - 150 for n > 2; a(0) = 357, a(1) =
 *          14450, a(2) = 221650.
 * https://oeis.org/A166913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2) - 150 for n > 2; a(0) = 357, a(1) =
 *            14450, a(2) = 221650
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)357;
    if (n == 1u) return (seq_t)14450;
    if (n == 2u) return (seq_t)221650;

    seq_t prevprev = (seq_t)14450; /* a(1) */
    seq_t prev     = (seq_t)221650; /* a(2) */

    for (idx_t i = 3u; i <= n; ++i)
    {
        seq_t cur = prev * (seq_t)20;
        cur -= prevprev * (seq_t)64;
        cur -= (seq_t)150;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
