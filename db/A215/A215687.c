/*
 * A215687  Number of solid standard Young tableaux of shape [[2*n,2],[2]].
 * https://oeis.org/A215687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n-1)*(2*n^2+7*n+7)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 180, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly to avoid unsigned wrap‑around. */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;

    /* Compute n * (2*n - 1) first to keep intermediate values small. */
    seq_t part1 = nn * (((seq_t)2 * nn) - 1);

    /* Compute (2*n^2 + 7*n + 7). */
    seq_t part2 = ((seq_t)2 * nn * nn) + ((seq_t)7 * nn) + 7;

    return part1 * part2;
}
