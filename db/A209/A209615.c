/*
 * A209615  Completely multiplicative with a(p^e) = 1 if p == 1 (mod 4),
 *          a(p^e) = (-1)^e otherwise.
 * https://oeis.org/A209615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(1) = 1; for n>1: if n even: a(n) = -a(n/2); if n odd: a(n) =
 *            (-1)^((n-1)/2)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* base case */
    if ((n & 1ULL) == 0)            /* n even */
        return -a(n >> 1);          /* a(2k) = -a(k) */
    else                            /* n odd, n = 2k+1 */
    {
        /* (-1)^k: 1 if k even, -1 if k odd */
        return ((n >> 1) & 1ULL) ? -1 : 1;
    }
}
