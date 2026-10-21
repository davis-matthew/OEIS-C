/*
 * A059134  A hierarchical sequence (W2{3}c - see A059126).
 * https://oeis.org/A059134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 21*ν_2(n+1) + 11 (ν_2 is the exponent of 2 in the
 *            factorisation of n+1)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Write n+1 as odd * 2^p and compute 21*p+11 */
    idx_t m = n + (idx_t)1;      /* n+1 (unsigned wrap‑around is fine for the test range) */

    /* Count how many trailing zeros in m, i.e. the exponent of 2 in n+1 */
    idx_t p = 0;
    while ((m & (idx_t)1) == 0) {
        ++p;
        m >>= 1;
    }

    /* Compute the value; cast to seq_t only after the multiplication to avoid
       overflow of the intermediate idx_t product.  21*p is at most
       21*64 (≈ 1350) for any 64‑bit n, far below seq_t limits. */
    seq_t value = (seq_t)(21 * (seq_t)p + 11);

    return value;
}
