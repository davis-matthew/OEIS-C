/*
 * A100523  a(n) = a(n-1)^2 + 2*a(n-1) - 1 with a(0) = 1.
 * https://oeis.org/A100523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + 2*a(n-1) - 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case */
    seq_t val = (seq_t)1;          /* a(0) */

    /* Iterate up to n, computing each term from the previous one */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t prev = val;
        val = prev * prev + (prev << 1) - (seq_t)1;
    }

    return val;
}
