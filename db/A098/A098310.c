/*
 * A098310  Unsigned member r=-11 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A098310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - a(n-2) + 2*(-1)^(n+1) Initial values: a(0)=0,
 *            a(1)=1 The sequence is generated iteratively; no memoisation is
 *            needed because each call recomputes up to n in O(n) time
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 13 * prev1 - prev2;
        if ((i + 1) & 1)      /* odd : (-1)^(i+1) = -1  */
            cur -= (seq_t)2;
        else                  /* even : (-1)^(i+1) = +1 */
            cur += (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
