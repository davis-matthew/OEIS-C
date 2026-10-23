/*
 * A020572  Expansion of g.f. 1/((1 - 6*x)*(1 - 7*x)*(1 - 10*x)).
 * https://oeis.org/A020572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 172*a(n-2) + 420*a(n-3); Initial values:
 *            a(0)=1, a(1)=23, a(2)=357
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)357;

    seq_t prev3 = (seq_t)1;   /* a(n-3) */
    seq_t prev2 = (seq_t)23;  /* a(n-2) */
    seq_t prev1 = (seq_t)357; /* a(n-1) */
    seq_t cur   = prev1;

    for (idx_t i = 3; i <= n; ++i) {
        /* compute a(i) = 23*a(i-1) - 172*a(i-2) + 420*a(i-3) */
        cur = (seq_t)23 * prev1 - (seq_t)172 * prev2 + (seq_t)420 * prev3;

        /* shift the window */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
