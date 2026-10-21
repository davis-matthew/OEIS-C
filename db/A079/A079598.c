/*
 * A079598  a(n) = 2^(4*n+1) - 2^(2*n).
 * https://oeis.org/A079598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4*n+1) - 2^(2*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 2^e for any e, using only unsigned arithmetic */
static seq_t pow2_u64(idx_t e)
{
    seq_t result = 1;      /* 2^0 */
    seq_t base   = 2;      /* 2^1 */

    /* Binary exponentiation.  Multiplication of unsigned types
     * is defined modulo 2^w, which is fine for the values that
     * actually fit into seq_t.  For larger exponents the function
     * is called only for terms that do not fit in seq_t, so the
     * wrapped result is irrelevant. */
    while (e) {
        if (e & 1U) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t high = pow2_u64(4 * n + 1);
    seq_t low  = pow2_u64(2 * n);
    return high - low;
}
