/*
 * A154347  a(n) = ( (7 + 2*sqrt(2))^n - (7 - 2*sqrt(2))^n )/(4*sqrt(2)).
 * https://oeis.org/A154347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 41*a(n-2) with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset   : 1   (a(1) is the first listed term) */

seq_t a(idx_t n)
{
    /* Handle the first two values directly */
    if (n == 0)
        return (seq_t)0;          /* value below offset – harmless */
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;       /* a(0) */
    seq_t prev1 = (seq_t)1;       /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)(14 * prev1 - 41 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
