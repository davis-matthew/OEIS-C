/*
 * A179123  a(n) is the number of red balls in an urn such that if the urn
 *          contains b(n)=A105045(n) blue balls, the probability of drawing 6
 *          red balls without replacement is the same as drawing 4 red and 2
 *          blue balls without replacement; that is, binomial(a(n),6) =
 *          binomial(a(n),4) * binomial(b(n),2).
 * https://oeis.org/A179123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 8*a(n+2) - a(n) - 27 ; with base values a(1)=5,
 *            a(2)=10, a(3)=14, a(4)=49
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the known base values for 1 ≤ n ≤ 4. */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)14;
    if (n == 4) return (seq_t)49;

    /* Variables hold the last four computed terms:
         t0 = a(i-4), t1 = a(i-3), t2 = a(i-2), t3 = a(i-1).
       They are initialized for i = 5 (i.e., for a(5)).             */
    seq_t t0 = (seq_t)5;   /* a(1)   */
    seq_t t1 = (seq_t)10;  /* a(2)   */
    seq_t t2 = (seq_t)14;  /* a(3)   */
    seq_t t3 = (seq_t)49;  /* a(4)   */
    seq_t result = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        /* Recurrence:  a(i) = 8*a(i-2) - a(i-4) - 27          */
        seq_t newv = (seq_t)8 * t2 - t0 - (seq_t)27;
        /* Slide the window forward for the next iteration.    */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = newv;
        result = newv;
    }

    return result;
}
