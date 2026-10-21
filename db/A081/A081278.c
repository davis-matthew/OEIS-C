/*
 * A081278  Binomial transform of Chebyshev polynomial coefficients A001793.
 * https://oeis.org/A081278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=29 a(n)=9*a(n‑1) – 27*a(n‑2) + 27*a(n‑3)
 *            for n≥3 This recurrence follows from a(n)=(n²+8n+9)·3^(n‑2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)29;

    /* we only need the previous three values */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)6;   /* a(1) */
    seq_t a2 = (seq_t)29;  /* a(2) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)9 * a2
            - (seq_t)27 * a1
            + (seq_t)27 * a0;
        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
