/*
 * A072201  a(n) = 4*a(n-1) + 1, a(1) = 15.
 * https://oeis.org/A072201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 1, with a(1) = 15
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A072201.
 *  idx_t : unsigned integer type for indices
 *  seq_t : unsigned integer type for sequence values (may be 8/16/32/64/128 bits or an arbitrary‑precision integer)
 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)15;

    seq_t val = (seq_t)15;
    for (idx_t i = 2; i <= n; ++i)
        val = val * (seq_t)4 + (seq_t)1;

    return val;
}
