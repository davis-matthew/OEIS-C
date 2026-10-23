/*
 * A163037  Number of n X 2 binary arrays with all 1s connected and a path of
 *          1s from left column to right column.
 * https://oeis.org/A163037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n‑1) – 8*a(n‑2) + 4*a(n‑3) + a(n‑4) – a(n‑5) with
 *            a(1)=1, a(2)=7, a(3)=28, a(4)=88, a(5)=245
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases – n is 1‑based */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)28;
    if (n == 4) return (seq_t)88;
    if (n == 5) return (seq_t)245;

    /* variables hold the last five values needed in the recurrence */
    seq_t v1 = (seq_t)1;    /* a(n‑5) */
    seq_t v2 = (seq_t)7;    /* a(n‑4) */
    seq_t v3 = (seq_t)28;   /* a(n‑3) */
    seq_t v4 = (seq_t)88;   /* a(n‑2) */
    seq_t v5 = (seq_t)245;  /* a(n‑1) */
    seq_t ai = v5;          /* placeholder for current term */

    /* compute terms from 6 up to n */
    for (idx_t i = 6; i <= n; ++i) {
        ai = 5 * v5 - 8 * v4 + 4 * v3 + v2 - v1;  /* recurrence */
        /* shift the window */
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
        v5 = ai;
    }
    return ai;
}
