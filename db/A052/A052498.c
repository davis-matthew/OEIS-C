/*
 * A052498  Number of nonsingular n X n matrices over GF(11).
 * https://oeis.org/A052498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (11^n – 11^k) (number of nonsingular n×n
 *            matrices over GF(11))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 11^n. */
    seq_t pow_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow_n *= (seq_t)11;

    /* Compute the product (11^n – 11^0)(11^n – 11^1)…(11^n – 11^(n-1)). */
    seq_t result = (seq_t)1;
    seq_t power  = (seq_t)1;          /* 11^0 initially. */
    for (idx_t k = 0; k < n; ++k) {
        result *= (pow_n - power);
        power  *= (seq_t)11;
    }
    return result;
}
