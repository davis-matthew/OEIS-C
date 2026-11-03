/*
 * A106258  Expansion of 1/sqrt(1-8x-8x^2).
 * https://oeis.org/A106258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence * a(0) = 1 * a(1) = 4 * for n ≥ 2: n * a(n) =
 *            4*(2*n-1)*a(n-1) + 8*(n-1)*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 191 (2026-11-03)
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
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t a_prevprev = (seq_t)1; /* a_wide(n-2) */
    seq_t a_prev     = (seq_t)4; /* a_wide(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff1 = (seq_t)4 * ((seq_t)2 * i - (seq_t)1);
        seq_t coeff2 = (seq_t)8 * ((seq_t)i - (seq_t)1);

        seq_t term1 = coeff1 * a_prev;
        seq_t term2 = coeff2 * a_prevprev;

        seq_t numerator = term1 + term2;
        seq_t cur = numerator / i;  /* division is exact by recurrence */

        a_prevprev = a_prev;
        a_prev = cur;
    }
    return a_prev;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
