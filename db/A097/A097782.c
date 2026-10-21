/*
 * A097782  Chebyshev polynomials S(n,29) with Diophantine property.
 * https://oeis.org/A097782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - a(n-2), with a(0)=1, a(1)=29
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the two initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;

    /* Iteratively compute the recurrence up to n.
       This works for any build of seq_t (fixed‑size or arbitrary‑precision). */
    seq_t a_prev_prev = (seq_t)1;   /* a(0) */
    seq_t a_prev = (seq_t)29;       /* a(1) */
    seq_t a_curr;                   /* placeholder for a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (seq_t)29 * a_prev - a_prev_prev; /* a(i) = 29*a(i-1) - a(i-2) */
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }
    return a_curr; /* a(n) */
}
