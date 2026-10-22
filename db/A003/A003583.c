/*
 * A003583  a(n) = (n+2)*2^(2*n-1) - (n/2)*binomial(2*n,n).
 * https://oeis.org/A003583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*2^(2*n-1) - n*binomial(2*n-1,n-1) for n >= 1
 *            (equivalent to (n+2)*2^(2*n-1) - (n/2)*binomial(2*n,n)), a(0) =
 *            1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c, p;
    idx_t k, i;
    if (n == 0) return (seq_t)1;
    /* c = binomial(2k-1, k-1), stepped from k = 1 up to k = n:
       binomial(2k+1, k) = binomial(2k-1, k-1) * 2*(2k+1) / (k+1), exact */
    c = (seq_t)1;
    for (k = 1; k < n; k++) {
        c = c * (seq_t)(4 * k + 2);
        c = c / (seq_t)(k + 1);
    }
    /* p = (n+2) * 2^(2n-1), by repeated doubling (wraps harmlessly in fixed width) */
    p = (seq_t)n + 2;
    for (i = 0; i + 1 < 2 * n; i++) {
        p += p;
    }
    return p - (seq_t)n * c;
}
