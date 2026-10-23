/*
 * A154027  a(n+2) = 100*a(n+1) - a(n), a(1)=0, a(2)=10.
 * https://oeis.org/A154027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 100*a(n+1) - a(n), with a(1)=0, a(2)=10
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known initial values */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)10;

    /* Iterate from the 3rd term onward */
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)10;  /* a(n-1) */
    seq_t cur = (seq_t)0;

    const seq_t C100 = (seq_t)100;  /* multiplier in the recurrence */

    /* n is an unsigned integer type; the loop ends safely */
    for (idx_t i = 3; i <= n; ++i) {
        cur = C100 * prev1 - prev2;  /* a(i) = 100*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;  /* a(n) */
}
