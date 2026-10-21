/*
 * A055541  Total number of leaves (nodes of vertex degree 1) in all labeled
 *          trees with n nodes.
 * https://oeis.org/A055541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (n-1)^(n-2) (for n > 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Compute (n-1)^(n-2) by repeated multiplication */
    seq_t base = (seq_t)(n - 1);
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i) {
        power *= base;
    }

    /* Final result: n * (n-1)^(n-2) */
    return (seq_t)n * power;
}
