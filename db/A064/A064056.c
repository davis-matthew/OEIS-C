/*
 * A064056  Seventh column of quintinomial coefficients.
 * https://oeis.org/A064056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*C(n+2,2) + 10*C(n+2,3) + 10*C(n+2,4) + 5*C(n+2,5) +
 *            C(n+2,6)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (C(m,k) is the binomial coefficient) */

/* compute binomial(m,k) for small k (k <= 6) exactly, using 128‑bit arithmetic for safety  */
static seq_t binom_small(idx_t m, idx_t k)
{
    if (k > m) return 0;
    if (k > m - k) k = m - k;        /* use symmetry */

    unsigned __int128 num = 1;
    unsigned __int128 den = 1;
    for (idx_t i = 1; i <= k; ++i) {
        num *= (unsigned __int128)(m - k + i);
        den *= (unsigned __int128)i;
    }
    /* division is exact */
    return num / den;
}

seq_t a(idx_t n)
{
    /* n is non‑negative (idx_t), we need n+2 as an index for the binomials   */
    idx_t m = n + 2;
    seq_t sum = 0;

    if (m >= 2) {
        seq_t c = binom_small(m, 2);
        sum += (seq_t)3 * c;
    }
    if (m >= 3) {
        seq_t c = binom_small(m, 3);
        sum += (seq_t)10 * c;
    }
    if (m >= 4) {
        seq_t c = binom_small(m, 4);
        sum += (seq_t)10 * c;
    }
    if (m >= 5) {
        seq_t c = binom_small(m, 5);
        sum += (seq_t)5 * c;
    }
    if (m >= 6) {
        seq_t c = binom_small(m, 6);
        sum += c;
    }
    return sum;
}
