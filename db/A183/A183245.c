/*
 * A183245  Number of permutations of 1..2*n+2 with each element displaced by
 *          at least n.
 * https://oeis.org/A183245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 12*a(n-2) for n>3 Initial values: a(1)=9,
 *            a(2)=29, a(3)=112
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle illegal or zero index – return 0 (undefined for 0) */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Base cases */
    if (n == 1) { return (seq_t)9; }
    if (n == 2) { return (seq_t)29; }
    if (n == 3) { return (seq_t)112; }

    /* Compute iteratively using the recurrence */
    seq_t prev2 = (seq_t)29;   /* a(2) */
    seq_t prev1 = (seq_t)112;  /* a(3) */
    seq_t cur;
    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 * (seq_t)7 - prev2 * (seq_t)12;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
