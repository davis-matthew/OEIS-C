/*
 * A157652  a(n) = 40*(200*n - 49).
 * https://oeis.org/A157652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*(200*n - 49)
 * Range:     u8 none, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All computations are performed in seq_t to avoid overflow. */
    seq_t sn = (seq_t)n;          /* cast index to the value type */
    seq_t val = 40 * (200 * sn - 49);
    return val;
}
