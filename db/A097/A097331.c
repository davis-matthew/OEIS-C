/*
 * A097331  Expansion of 1 + 2x/(1 + sqrt(1 - 4x^2)).
 * https://oeis.org/A097331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n + Catalan((n-1)/2)(1-(-1)^n)/2 (i.e. a(n) = 0 for
 *            even n > 0, a(0)=1, and a(n)=Catalan((n-1)/2) for odd n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 74, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t catalan(idx_t k) {
    if (k == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (k + i) then divide by i, exact division */
        res = res * ((seq_t)k + i);
        res = res / i;
    }
    res = res / ((seq_t)k + 1);   /* divide by (k+1) to get Catalan */
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if ((n & 1) == 0) return (seq_t)0;  /* even n > 0 */
    idx_t k = (n - 1) >> 1;               /* (n-1)/2 */
    return catalan(k);
}
