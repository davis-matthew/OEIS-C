/*
 * A130253  Number of Jacobsthal numbers (A001045) <=n.
 * https://oeis.org/A130253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_2(3n+1)) + 1
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3*n+1 in seq_t to avoid overflow in idx_t */
    seq_t x = (seq_t)n * 3 + 1;
    seq_t k = 0;
    while (x > 0)
    {
        x >>= 1;
        ++k;
    }
    return k;
}
