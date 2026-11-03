/*
 * A100320  A Catalan transform of (1 + 2*x)/(1 - 2*x).
 * https://oeis.org/A100320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * binomial(2*n-1, n), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 949 (2026-11-03)
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

seq_t a_wide(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the binomial coefficient iteratively:
           res = res * (n-1+i) / i;  division is exact. */
        res = res * ((seq_t)(n - 1 + i)) / (seq_t)i;
    }
    return res * (seq_t)4;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
