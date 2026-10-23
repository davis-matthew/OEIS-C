/*
 * A166136  a(n) = n*(n+3)/2 + 7.
 * https://oeis.org/A166136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+3)/2 + 7
 * Range:     u8 n <= 14, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the seq_t type first to avoid narrowing. */
    seq_t nn = (seq_t)n;
    /* Compute n*(n+3)/2 in a straightforward way. */
    seq_t val = nn * (nn + 3);
    val /= 2;
    val += 7;
    return val;
}
