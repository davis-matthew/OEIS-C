/*
 * A082799  Smallest multiple of 3 beginning with n.
 * https://oeis.org/A082799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n if n mod 3 == 0 a(n) = n*10 + (3 - n mod 3) otherwise
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    idx_t r = n % 3;
    if (r == 0)
        return ns;                           /* n itself is multiple of 3 */
    return ns * 10 + (seq_t)(3 - r);          /* append 1 or 2 to get mod‑3 = 0 */
}
