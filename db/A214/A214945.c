/*
 * A214945  Number of squarefree words of length 6 in an (n+1)-ary alphabet.
 * https://oeis.org/A214945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + n^5 - 3*n^4 - 2*n^3 + 2*n^2 + n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t once */
    seq_t sn = (seq_t)n;

    /* compute successive powers of n */
    seq_t n2 = sn * sn;       /* n^2 */
    seq_t n3 = n2 * sn;       /* n^3 */
    seq_t n4 = n3 * sn;       /* n^4 */
    seq_t n5 = n4 * sn;       /* n^5 */
    seq_t n6 = n5 * sn;       /* n^6 */

    /* evaluate the polynomial */
    seq_t res = n6;       /* n^6 */
    res += n5;            /* + n^5 */
    res -= 3 * n4;        /* - 3*n^4 */
    res -= 2 * n3;        /* - 2*n^3 */
    res += 2 * n2;        /* + 2*n^2 */
    res += sn;            /* + n */

    return res;
}
