/*
 * A137500  Binomial transform of b(n) = (0, 0, A007910).
 * https://oeis.org/A137500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A137500: Binomial transform of b(n) = (0, 0, A007910) */
/* Recurrence: a(0)=0, a(1)=0, a(2)=1; for n>=3, 
   a(n) = 5*a(n-1) - 8*a(n-2) + 6*a(n-3) */

seq_t a(idx_t n)
{
    /* Handle initial values explicitly */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Use three rolling variables to compute the recurrence */
    seq_t prev3 = 0;   /* a(n-3) */
    seq_t prev2 = 0;   /* a(n-2) */
    seq_t prev1 = 1;   /* a(n-1) */
    seq_t curr;        /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        curr = 5 * prev1 - 8 * prev2 + 6 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
