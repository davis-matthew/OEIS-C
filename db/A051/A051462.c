/*
 * A051462  Molien series for group G_{1,2}^{8} of order 1536.
 * https://oeis.org/A051462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1 + 2*(2*n + 1)*(n^2 + n + 4) - (n mod 3))/9
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                     /* n^2 */
    seq_t n2pn = n2 + nn + (seq_t)4;        /* n^2 + n + 4 */
    seq_t two_n_plus1 = (seq_t)2 * nn + (seq_t)1; /* 2*n + 1 */
    seq_t temp = two_n_plus1 * n2pn;        /* (2*n+1)*(n^2+n+4) */
    seq_t mult = (seq_t)2 * temp;           /* 2*(2*n+1)*(n^2+n+4) */
    seq_t mod3 = (seq_t)(n % 3);            /* n mod 3 */
    seq_t num = (seq_t)1 + mult - mod3;     /* numerator */
    return num / (seq_t)9;                  /* exact division */
}
