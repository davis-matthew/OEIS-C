/*
 * A090012  Permanent of (0,1)-matrix of size n X (n+d) with d=2 and n-1 zeros
 *          not on a line.
 * https://oeis.org/A090012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=9, a(n) = (n+1)*a(n-1) + (n-2)*a(n-2) for n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* Iterate the recurrence.  Use only two state variables to
       keep the memory footprint small. */
    seq_t prev2 = (seq_t)3;   /* a(1)   */
    seq_t prev1 = (seq_t)9;   /* a(2)   */
    seq_t cur;                /* a(i)   */

    for (idx_t i = 3; i <= n; ++i) {
        cur = ((seq_t)(i + 1)) * prev1 + ((seq_t)(i - 2)) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
