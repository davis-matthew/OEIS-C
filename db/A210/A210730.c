/*
 * A210730  a(n) = a(n-1) + a(n-2) + n + 2 with n>1, a(0)=a(1)=0.
 * https://oeis.org/A210730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(n)=a(n-1)+a(n-2)+n+2 for n>1
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A210730: a(n) = a(n-1) + a(n-2) + n + 2, a(0)=a(1)=0  */
seq_t a(idx_t n)
{
    /* Base cases, a(0) = a(1) = 0 */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* Variables for the last two computed values:
       prev2 = a(i-2), prev1 = a(i-1)  */
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)0;   /* a(1) */
    seq_t cur   = (seq_t)0;   /* temporary storage for a(i) */

    /* Iterate from i = 2 up to n, applying the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 + prev2 + (seq_t)i + (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }

    /* When the loop finishes, prev1 holds a(n) */
    return prev1;
}
