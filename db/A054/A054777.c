/*
 * A054777  a(n) = 4*n*(4*n-1)*(4*n-2)*(4*n-3).
 * https://oeis.org/A054777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n*(4*n-1)*(4*n-2)*(4*n-3)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 64, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0  (a(0) = 0) */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly to avoid wrap‑around with unsigned arithmetic. */
    if (n == 0)
        return 0;

    /* Compute 4*n first */
    seq_t x = (seq_t)n * 4;

    /* Compute the product of the four consecutive integers starting at (4*n-3). */
    seq_t res = x * (x - 1);
    res = res * (x - 2);
    res = res * (x - 3);

    return res;
}
