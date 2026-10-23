/*
 * A015509  a(1) = 1, a(n) = Sum_{k=1..n-1} ((8^k - 1)/7)*a(k).
 * https://oeis.org/A015509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(n)=Sum_{k=1}^{n-1} ((8^k-1)/7)*a(k), which for
 *            n>=3 simplifies to a(n)=((8^(n-1)+6)/7)*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1; /* a(1) and a(2) are both 1 */
    if (n == 1)
        return result;
    if (n == 2)
        return result;

    seq_t pow8 = 8; /* holds 8^(i-1), starting with i=3: 8^2 after the first multiply */
    for (idx_t i = 3; i <= n; ++i)
    {
        pow8 = pow8 * 8;               /* now 8^(i-1) */
        seq_t factor = (pow8 + 6) / 7; /* integer division, exact */
        result = result * factor;
    }
    return result;
}
