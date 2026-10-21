/*
 * A215634  a(n) = - 6*a(n-1) - 9*a(n-2) - 3*a(n-3) with a(0)=3, a(1)=-6,
 *          a(2)=18.
 * https://oeis.org/A215634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=3, a(1)=-6, a(2)=18; a(n) = -6*a(n-1) - 9*a(n-2) - 3*a(n-3)
 *            for n ≥ 3
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0U) return (seq_t)3;
    if (n == 1U) return (seq_t)-6;
    if (n == 2U) return (seq_t)18;

    /* Iteratively compute subsequent terms using only three temporaries.
       The recurrence is linear with constant coefficients, so an O(n)
       method with O(1) memory is sufficient for all required ranges. */
    seq_t a0 = (seq_t)3;   /* a(n-3)   */
    seq_t a1 = (seq_t)-6;  /* a(n-2)   */
    seq_t a2 = (seq_t)18;  /* a(n-1)   */
    seq_t a3;              /* a(n)     */

    for (idx_t i = 3U; i <= n; ++i) {
        a3 = (seq_t)(-6 * a2 - 9 * a1 - 3 * a0); /* compute a(i) */
        a0 = a1; /* shift window */
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
