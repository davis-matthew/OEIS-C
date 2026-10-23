/*
 * A026049  a(n) = dot_product(1,2,...,n)*(7,8,...,n,1,2,3,4,5,6).
 * https://oeis.org/A026049
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2n^2-15n+109)/6
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
        return 0; /* terms below the offset are undefined */

    seq_t n_t = (seq_t)n;
    seq_t sq  = n_t * n_t;            /* n^2 */
    seq_t term = 2 * sq - 15 * n_t + 109;  /* 2n^2 - 15n + 109 */
    return n_t * term / 6;                    /* n*(...) / 6 */
}
