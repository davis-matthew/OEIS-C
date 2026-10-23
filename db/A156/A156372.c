/*
 * A156372  Denominator of Euler(n, 1/15).
 * https://oeis.org/A156372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^{v_2(n+1)} * 15^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the exponent of 2 in (n+1) */
    idx_t m = n + 1;
    idx_t e = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        e++;
    }

    /* compute 15^n */
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        val *= (seq_t)15;
    }

    /* multiply by 2^e */
    for (idx_t i = 0; i < e; ++i) {
        val *= (seq_t)2;
    }

    return val;
}
