/*
 * A140659  a(n) = floor(A140657(n+2)/10).
 * https://oeis.org/A140659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A140659
 * a(0) = 0
 * a(1) = 0
 * a(2n+1) = 2*a(2n)
 * a(2n+2) = 4*a(2n) + 1 + ((n)&1)   // A000034(n) = 1 for n even, 2 for n odd
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 0;

    seq_t prev2 = 0;     // a(i-2)
    seq_t prev1 = 0;     // a(i-1)
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (i & 1)           /* i is odd: a(i)=2*a(i-1) */
        {
            cur = ((seq_t)2) * prev1;
        }
        else                 /* i is even: a(i)=4*a(i-2)+1+((i/2-1)&1) */
        {
            idx_t k = (i - 2) >> 1;
            seq_t term = (seq_t)(1 + (k & 1));
            cur = ((seq_t)4) * prev2 + term;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
