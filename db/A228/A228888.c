/*
 * A228888  a(n) = binomial(3*n+2,3).
 * https://oeis.org/A228888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n+2,3) = (1/6)*(3*n)*(3*n + 1)*(3*n + 2)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 553, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3*n first to avoid repeated multiplications */
    seq_t n3 = (seq_t)n * 3;

    /* Compute product (3n)*(3n+1)*(3n+2) */
    seq_t prod = n3 * (n3 + 1);      /* (3n)*(3n+1)   */
    prod = prod * (n3 + 2);          /* *(3n+2)       */

    /* Divide by 6; the result is always an integer */
    prod /= 6;

    return prod;
}
