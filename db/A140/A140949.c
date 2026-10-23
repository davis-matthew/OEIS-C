/*
 * A140949  a(n) = number of iterations of k -> k-1/k until we reach a
 *          negative number, starting at n.
 * https://oeis.org/A140949
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊(n²‑1)/2⌋ + offset(n) offset(n) = 2 if n == 2 2 if n is
 *            odd and n ≤ 7 3 if n is odd and n ≥ 9 3 if n is even and 4 ≤ n ≤
 *            24 4 if n is even and n ≥ 26 (for all n ≥ 2)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                      /* offset starts at n = 2 */

    seq_t sq    = (seq_t)n * (seq_t)n;        /* n²   */
    seq_t base  = (sq - (seq_t)1) / (seq_t)2; /* ⌊(n²‑1)/2⌋ */

    int offset;
    if (n == (idx_t)2)
        offset = 2;
    else if (n & (idx_t)1)                   /* odd n */
        offset = (n <= (idx_t)7) ? 2 : 3;
    else                                     /* even n */
        offset = (n <= (idx_t)24) ? 3 : 4;

    return base + (seq_t)offset;
}
