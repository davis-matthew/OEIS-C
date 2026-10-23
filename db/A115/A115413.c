/*
 * A115413  Expansion of (x - 1)/(1 - x^2 + x^3 + x^4 - x^5).
 * https://oeis.org/A115413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) - a(n-3) - a(n-4) + a(n-5), for n >= 5; initial
 *            terms: a(0) = -1, a(1) = 1, a(2) = -1, a(3) = 2, a(4) = -1
 * Range:     i8 n <= 43, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle base cases directly */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)-1;

    /* ring of the last five values: t0=a(n-5), t1=a(n-4), t2=a(n-3),
       t3=a(n-2), t4=a(n-1) */
    seq_t t0 = (seq_t)-1; /* a(0) */
    seq_t t1 = (seq_t)1;  /* a(1) */
    seq_t t2 = (seq_t)-1; /* a(2) */
    seq_t t3 = (seq_t)2;  /* a(3) */
    seq_t t4 = (seq_t)-1; /* a(4) */
    seq_t t5; /* to hold a(k) */

    for (idx_t i = 5; i <= n; ++i) {
        t5 = t3 - t2 - t1 + t0;  /* a(i) */
        /* shift the ring for next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
    }
    return t4; /* when loop ends, t4 == a(n) */
}
