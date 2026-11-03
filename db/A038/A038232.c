/*
 * A038232  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*2^j.
 * https://oeis.org/A038232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (i,j)-entry of the triangle T(i,j) =
 *            binom(i,j)*4^(i-j)*2^j, read in row‑major order. i =
 *            floor((sqrt(1+8n)-1)/2), j = n - i*(i+1)/2
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 */
    idx_t i = 0;
    while ((i * (i + 1)) / 2 <= n)
        ++i;
    --i;
    idx_t j = n - (i * (i + 1)) / 2;

    /* t(i,0) = 4^i */
    seq_t val = pow4(i);

    /* Compute t(i,j) multiplicatively:
       t(i,j+1) = t(i,j) * (i-j) / (2*(j+1)) */
    for (idx_t k = 0; k < j; ++k) {
        val = val * (seq_t)(i - k) / ((seq_t)2 * (seq_t)(k + 1));
    }
    return val;
}
