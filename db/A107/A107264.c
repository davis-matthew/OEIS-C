/*
 * A107264  Expansion of (1 - 3*x - sqrt((1-3*x)^2 - 4*3*x^2))/(2*3*x^2).
 * https://oeis.org/A107264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, for n≥2 a(n) = [3*(2*n+1)*a(n-1) +
 *            3*(n-1)*a(n-2)] / (n+2) (exact division)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 199 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a0 = (seq_t)1;            /* a_wide(0) */
    seq_t a1 = (seq_t)3;            /* a_wide(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t idx = (seq_t)i;
        /* numerator = 3*(2*i+1)*a1 + 3*(i-1)*a0 */
        seq_t term1 = a1 * ((seq_t)3 * ((seq_t)2 * idx + (seq_t)1));
        seq_t term2 = a0 * ((seq_t)3 * (idx - (seq_t)1));
        seq_t num = term1 + term2;
        /* division is guaranteed to be exact */
        a0 = a1;
        a1 = num / (idx + (seq_t)2);
    }
    return a1;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
