/*
 * A016150  Expansion of g.f. 1/((1-4*x)*(1-7*x)).
 * https://oeis.org/A016150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n+1) - 4^(n+1))/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence for all n >= 0:
 *   a(0) = 1
 *   a(n) = 4 * a(n-1) + 7^n   (for n >= 1)            */

seq_t a(idx_t n)
{
    /* handle the base case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* iterate using the recurrence a(n) = 4*a(n-1) + 7^n */
    seq_t prev = (seq_t)1;          /* a(0)   */
    seq_t cur  = (seq_t)0;          /* placeholder for a(n)  */
    seq_t pow7 = (seq_t)1;          /* holds 7^i during the loop */

    for (idx_t i = 1; i <= n; ++i) {
        pow7 *= (seq_t)7;                   /* pow7 = 7^i   */
        cur = prev * (seq_t)4 + pow7;       /* a(i) = 4*a(i-1)+7^i */
        prev = cur;                         /* move to next step */
    }

    return cur;
}
