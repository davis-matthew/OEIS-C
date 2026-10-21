/*
 * A054324  Sixth unsigned column of Lanczos triangle A053125 (decreasing
 *          powers).
 * https://oeis.org/A054324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * binomial(2*n + 6, 5)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 8, u64 n <= 21, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial(2*n + 6, 5) multiplicatively */
    idx_t M = 2 * n + 6;          /* upper parameter of the binomial */
    seq_t binom = 1;              /* start value */

    for (idx_t i = 1; i <= 5; ++i) {
        binom *= (M - (i - 1));    /* multiply by decreasing term */
        binom /= i;                /* divide by i (exact division) */
    }

    /* Multiply by 4^n: simple loop, n <= 10000 is fine */
    seq_t result = binom;
    for (idx_t k = 0; k < n; ++k) {
        result *= 4;
    }

    return result;
}
