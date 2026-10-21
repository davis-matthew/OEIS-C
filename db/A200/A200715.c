/*
 * A200715  Expansion of (-3*x^2 + x - 1)/(x^3 - 3*x^2 + x - 1).
 * https://oeis.org/A200715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 3*a(n-2) + a(n-3), with a(0)=1, a(1)=0, a(2)=0
 * Range:     i8 n <= 13, i16 n <= 23, i32 n <= 45, i64 n <= 89, i128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly to avoid infinite loops for small n. */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;

    /* Use three rolling variables to compute the recurrence iteratively. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)0;   /* a(2) */
    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        next = a2 - (seq_t)3 * a1 + a0;  /* a(i) = a(i-1) - 3*a(i-2) + a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
