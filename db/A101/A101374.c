/*
 * A101374  a(n) = n*(n^3 - n + 2)/2.
 * https://oeis.org/A101374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 - n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=1, a(2)=8, a(3)=39, a(4)=124 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Fast path for n = 0 to avoid unnecessary work */
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    /* Inside the parenthesis is always even, so division by 2 is exact */
    seq_t t   = n3 - nn + (seq_t)2; /* n^3 - n + 2 */
    seq_t res = nn * (t / (seq_t)2);/* n * (n^3 - n + 2) / 2 */
    return res;
}
