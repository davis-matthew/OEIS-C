/*
 * A077961  Expansion of 1 / (1 + x^2 - x^3) in powers of x.
 * https://oeis.org/A077961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-3) - a(n-2); a(0)=1, a(1)=0, a(2)=-1
 * Range:     i8 n <= 26, i16 n <= 55, i32 n <= 114, i64 n <= 231,
 *            i128 n <= 461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-1;

    /* Iteratively compute using the recurrence a(n) = a(n-3) - a(n-2) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)-1;  /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = a0 - a1;  /* a(i) = a(i-3) - a(i-2) */
        a0 = a1;       /* shift window for next step */
        a1 = a2;
        a2 = an;
    }
    return a2;
}
