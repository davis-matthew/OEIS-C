/*
 * A077953  Expansion of 1/(1-x+2*x^2-2*x^3).
 * https://oeis.org/A077953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=-1; a(n) = a(n-1) - 2*a(n-2) + 2*a(n-3) for
 *            n >= 3
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values */
    const seq_t a0 = (seq_t)1; /* a(0) */
    const seq_t a1 = (seq_t)1; /* a(1) */
    const seq_t a2 = (seq_t)-1;/* a(2) */

    /* return directly for n < 3 */
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    /* iterate the recurrence up to index n */
    seq_t prev3 = a0; /* a(i-3)  */
    seq_t prev2 = a1; /* a(i-2)  */
    seq_t prev1 = a2; /* a(i-1)  */
    seq_t cur   ;      /* a(i)    */

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 - (seq_t)2 * prev2 + (seq_t)2 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur; /* a(n) */
}
