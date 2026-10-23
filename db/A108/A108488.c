/*
 * A108488  Expansion of 1/sqrt(1 -2*x -3*x^2 -4*x^3 +4*x^4).
 * https://oeis.org/A108488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (OEIS A108488):  a(n) = Σ_{k=0..n} C(n-k, k)^2 · 2^k
   Recurrence:  a(0)=1, a(1)=1, a(2)=3, a(3)=9
   For n≥4:
      n·a(n) = (2n-1)a(n-1) + (3n-3)a(n-2) + (4n-6)a(n-3) – (4n-8)a(n-4)
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)9;

    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)3;   /* a(n-2) */
    seq_t a3 = (seq_t)9;   /* a(n-1) */

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
