/*
 * A093040  Expansion of (1+x)/((1+x+x^2)(1-x-x^2)).
 * https://oeis.org/A093040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093040: a(n) = a(n-2) + 2*a(n-3) + a(n-4)
   with a(0)=1, a(1)=1, a(2)=1, a(3)=3 */
seq_t a(idx_t n) {
    /* Small n: return the base values directly. */
    if (n < 3) return (seq_t)1;      /* a(0)=a(1)=a(2)=1 */
    if (n == 3) return (seq_t)3;      /* a(3)=3 */

    /* Iteratively apply the recurrence for n ≥ 4.
       Keep only the last four values: a(n‑4), a(n‑3), a(n‑2), a(n‑1). */
    seq_t an_minus_4 = (seq_t)1;   /* a(0)   */
    seq_t an_minus_3 = (seq_t)1;   /* a(1)   */
    seq_t an_minus_2 = (seq_t)1;   /* a(2)   */
    seq_t an_minus_1 = (seq_t)3;   /* a(3)   */
    seq_t an;                      /* a(n)   */

    for (idx_t i = 4; i <= n; ++i) {
        an = an_minus_2 + (an_minus_3 * (seq_t)2) + an_minus_4;
        /* shift the window: a(n‑4)←a(n‑3), a(n‑3)←a(n‑2), a(n‑2)←a(n‑1), a(n‑1)←a(n) */
        an_minus_4 = an_minus_3;
        an_minus_3 = an_minus_2;
        an_minus_2 = an_minus_1;
        an_minus_1 = an;
    }
    return an;
}
