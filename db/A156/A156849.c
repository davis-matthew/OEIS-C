/*
 * A156849  Numbers k such that k^2 == 2 (mod 23^2).
 * https://oeis.org/A156849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 156 + 529*((n-1)/2) + (n%2==0 ? 217 : 0)
 * Range:     u8 n <= 1, u16 n <= 248, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term (offset 1) of OEIS A156849: numbers k such that k^2 ≡ 2 (mod 23^2). */
seq_t a(idx_t n)
{
    if (n == 0)       /* undefined for n < 1, return 0 safely */
        return (seq_t)0;

    idx_t k = (n - 1) / 2;                     /* integer division */
    seq_t res = (seq_t)156 + (seq_t)529 * (seq_t)k;
    if ((n & 1) == 0)                         /* even n → add 217 */
        res += (seq_t)217;
    return res;
}
