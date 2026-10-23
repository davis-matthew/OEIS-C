/*
 * A028182  Expansion of 1/((1-5*x)*(1-7*x)*(1-8*x)*(1-11*x)).
 * https://oeis.org/A028182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n+1)-5^(n+1))/2+19*a(n-1)-88*a(n-2) with a(0) = 1,
 *            a(1) = 31
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first two known terms immediately. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;

    /* Variables holding a(n-2), a(n-1) and the current term. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)31;  /* a(1) */
    seq_t curr;

    /* Powers needed for the recurrence:
       We need 5^(n+1) and 7^(n+1).  Start with n = 2. */
    seq_t p5 = (seq_t)125;  /* 5^3, for n = 2 */
    seq_t p7 = (seq_t)343;  /* 7^3, for n = 2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = (p7 - p5)/2 + 19*prev1 - 88*prev2 */
        curr = (p7 - p5) / (seq_t)2
             + (seq_t)19 * prev1 
             - (seq_t)88 * prev2;

        /* Prepare values for the next iteration. */
        p5 *= (seq_t)5;    /* 5^(i+2) for next i+1 */
        p7 *= (seq_t)7;    /* 7^(i+2) for next i+1 */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
