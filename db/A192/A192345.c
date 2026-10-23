/*
 * A192345  Constant term of the reduction of n-th polynomial at A161516 by
 *          x^2->x+1.
 * https://oeis.org/A192345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=0, a(2)=1, a(3)=2, a(4)=17.
 * For n ≥ 5:
 *   a(n) = 2*a(n-1) + 10*a(n-2) – 6*a(n-3) – 9*a(n-4)
 *
 * The recurrence follows from the empirical generating function
 *   x^2*(3*x^2+1) / (9*x^4+6*x^3-10*x^2-2*x+1)
 */
seq_t a(idx_t n)
{
    /* Handle the four known initial values. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)17;

    /* Use a sliding window of the last four values. */
    seq_t v1 = (seq_t)0;   /* a(n-4) */
    seq_t v2 = (seq_t)1;   /* a(n-3) */
    seq_t v3 = (seq_t)2;   /* a(n-2) */
    seq_t v4 = (seq_t)17;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = v4 * (seq_t)2
                   + v3 * (seq_t)10
                   - v2 * (seq_t)6
                   - v1 * (seq_t)9;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return v4;
}
