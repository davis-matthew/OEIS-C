/*
 * A120758  The (1,3)-entry in the matrix M^n, where M is the 3 X 3 matrix
 *          [0,2,1; 2,1,2; 1,2,2] (n>=1).
 * https://oeis.org/A120758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+7*a(n-2)-a(n-3) for n>=4, with initial values
 *            a(1)=1, a(2)=6, a(3)=25
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial and base cases.  Index 1 is the first term. */
    if (n == 0) return (seq_t)0;     /* undefined, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)25;

    /* Iterate using the recurrence relation. */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)6;   /* a(n-2) */
    seq_t a3 = (seq_t)25;  /* a(n-1) */
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(n) = 3*a(n-1) + 7*a(n-2) - a(n-3)  */
        a4 = (seq_t)3 * a3 + (seq_t)7 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a3;
}
