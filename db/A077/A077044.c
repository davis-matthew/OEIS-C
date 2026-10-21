/*
 * A077044  Largest coefficient in expansion of (1 + x + x^2 + ... +
 *          x^(n-1))^5 = ((1-x^n)/(1-x))^5, i.e., the coefficient of
 *          x^floor(5*(n-1)/2) and of x^ceiling(5*(n-1)/2); also number of
 *          compositions of floor(5*(n+1)/2) into exactly 5 positive integers
 *          each no more than n.
 * https://oeis.org/A077044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (230*n^4 + 70*n^2 + 27 - (30*n^2 + 27)*(-1)^n)/384
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 289, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = n;
    seq_t n2    = n_seq * n_seq;          /* n^2 */
    seq_t n4    = n2 * n2;                /* n^4 */
    seq_t term  = 230 * n4 + 70 * n2 + 27;
    seq_t s     = 30 * n2 + 27;
    seq_t num;

    /* (-1)^n is 1 for even n, -1 for odd n.
       For even n subtract s; for odd n add s. */
    if ((n & 1) == 0)
        num = term - s;
    else
        num = term + s;

    return num / 384;
}
