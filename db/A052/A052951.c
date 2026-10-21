/*
 * A052951  Expansion of (1 + x - 2*x^2)/(1 - 2*x)^2.
 * https://oeis.org/A052951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = (n + 1)·2^n + 2^(n-1) for n > 0 (OEIS A052951)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle offset */
    if (n == 0) return (seq_t)1;

    /* Compute 2^n by repeated doubling (no undefined shift width) */
    seq_t pow2n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2n = pow2n << 1;   /* unsigned left shift is well‑defined modulo 2^w */
    }

    /* 2^(n-1) is pow2n >> 1; safe because n > 0 */
    seq_t pow2n_minus = pow2n >> 1;

    /* (n + 1) * 2^n + 2^(n-1) */
    seq_t term = (seq_t)(n + 1) * pow2n;  /* cast n+1 to seq_t */
    return term + pow2n_minus;
}
