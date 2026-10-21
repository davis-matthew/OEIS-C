/*
 * A099273  Unsigned member r=-15 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - a(n-2) + 2*(-1)^(n+1), a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)0;      /* a(n-2) */
    seq_t a1 = (seq_t)1;      /* a(n-1) */
    seq_t a2;                 /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a2 = 17*a1 - a0 + 2*(-1)^(i+1) */
        a2 = (seq_t)17 * a1 - a0;

        /* (-1)^(i+1) is -1 when i is even, +1 when i is odd */
        if ((i & 1) == 0)          /* i even -> -2 */
            a2 -= (seq_t)2;
        else                        /* i odd  -> +2 */
            a2 += (seq_t)2;

        /* Shift the sliding window */
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
