/*
 * A018913  a(n) = 9*a(n - 1) - a(n - 2) for n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A018913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0ULL) return (seq_t)0;
    if (n == 1ULL) return (seq_t)1;

    /* Two most recent terms: a(n-2) and a(n-1) */
    seq_t a_prev2 = (seq_t)0;  /* a(0) */
    seq_t a_prev1 = (seq_t)1;  /* a(1) */
    seq_t a_cur;

    /* Iterate up to the requested index */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute 9*a_prev1 - a_prev2 using only seq_t arithmetic */
        a_cur = (seq_t)( (seq_t)9 * a_prev1 - a_prev2 );
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_prev1;  /* a(n) */
}
