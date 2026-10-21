/*
 * A077863  Expansion of (1-x)^(-1)/(1-x-2*x^2-2*x^3).
 * https://oeis.org/A077863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(n)=a(n-1)+2*a(n-2)+2*a(n-3)+1 for n≥3
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    /* Use three previous values to iterate up to n */
    seq_t prev2 = (seq_t)1; /* a(n-3) */
    seq_t prev1 = (seq_t)2; /* a(n-2) */
    seq_t curr  = (seq_t)5; /* a(n-1) */

    /* Compute successive terms */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = curr
                    + (seq_t)2 * prev1
                    + (seq_t)2 * prev2
                    + (seq_t)1;

        /* Shift the window */
        prev2 = prev1;
        prev1 = curr;
        curr  = next;
    }

    return curr;
}
