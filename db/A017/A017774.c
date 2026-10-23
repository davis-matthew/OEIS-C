/*
 * A017774  Binomial coefficients C(58,n).
 * https://oeis.org/A017774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(58, n)
 * (coefficients of (1+x)^58; a(n)=0 for n>58)
 */
seq_t a(idx_t n)
{
    /* If the index is outside the row, the value is zero. */
    if (n > (idx_t)58)
        return (seq_t)0;

    /* Use symmetry to keep the loop short: k = min(n, 58 - n) */
    idx_t k = n;
    if (n > (idx_t)58 / 2)
        k = (idx_t)58 - n;

    seq_t result = (seq_t)1;
    /* Compute product (58 - k + 1) * ... * 58 / 1 * ... * k */
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Multiply first to keep the division exact */
        result *= (seq_t)(58 - k + i);
        result /= (seq_t)i;
    }

    return result;
}
