/*
 * A108488  Expansion of 1/sqrt(1 -2*x -3*x^2 -4*x^3 +4*x^4).
 * https://oeis.org/A108488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 77,
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

/* 
   Formula (OEIS A108488):  a_wide(n) = Σ_{k=0..n} C(n-k, k)^2 · 2^k
   Recurrence:  a_wide(0)=1, a_wide(1)=1, a_wide(2)=3, a_wide(3)=9
   For n≥4:
      n·a_wide(n) = (2n-1)a_wide(n-1) + (3n-3)a_wide(n-2) + (4n-6)a_wide(n-3) – (4n-8)a_wide(n-4)
*/
seq_t a_wide(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)9;

    seq_t a0 = (seq_t)1;   /* a_wide(n-4) */
    seq_t a1 = (seq_t)1;   /* a_wide(n-3) */
    seq_t a2 = (seq_t)3;   /* a_wide(n-2) */
    seq_t a3 = (seq_t)9;   /* a_wide(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = (seq_t)(2*i-1) * a3
                   + (seq_t)(3*i-3) * a2
                   + (seq_t)(4*i-6) * a1;
        term -= (seq_t)(4*i-8) * a0;        /* subtraction keeps unsigned safe */
        seq_t ai = term / (seq_t)i;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
