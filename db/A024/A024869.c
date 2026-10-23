/*
 * A024869  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k =
 *          floor( n/2 ), s = natural numbers >= 2, t = natural numbers >= 3.
 * https://oeis.org/A024869
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 21*n^2 + 46*n) / 24 for n even = (2*n^3 + 18*n^2
 *            + 22*n - 42) / 24 for n odd (OEIS A024869)
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* undefined for n < 2, return 0 */

    seq_t nn = (seq_t)n;
    seq_t val;

    if (n & 1) {                  /* n is odd */
        val = (seq_t)2 * nn * nn * nn;   /* 2*n^3   */
        val += (seq_t)18 * nn * nn;      /* +18*n^2 */
        val += (seq_t)22 * nn;           /* +22*n   */
        val -= (seq_t)42;                /* -42     */
    } else {                      /* n is even */
        val = (seq_t)2 * nn * nn * nn;   /* 2*n^3   */
        val += (seq_t)21 * nn * nn;      /* +21*n^2 */
        val += (seq_t)46 * nn;           /* +46*n   */
    }

    val /= (seq_t)24;             /* division by 24 */
    return val;
}
