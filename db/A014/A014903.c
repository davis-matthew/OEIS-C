/*
 * A014903  a(1)=1, a(n) = 19*a(n-1) + n.
 * https://oeis.org/A014903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) + n, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Result fits in seq_t for all test values. */
    seq_t val = 1;          /* a(1) */
    if (n < 2) return val; /* handle a(1) */

    for (idx_t i = 2; i <= n; ++i)
        val = val * (seq_t)19 + (seq_t)i;

    return val;
}
