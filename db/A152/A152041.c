/*
 * A152041  a(n) = A008893(n)/2.
 * https://oeis.org/A152041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n^2+7*n+2)/8
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 264, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for all intermediate calculations */
    seq_t n_t = (seq_t)n;
    seq_t m = n_t + 1;
    seq_t t = (seq_t)7 * n_t * n_t + (seq_t)7 * n_t + (seq_t)2;

    /* divide by 2 three times, distributing the divisions among the
       three factors to avoid intermediate overflow. */
    for (int i = 0; i < 3; ++i) {
        if ((n_t & (seq_t)1) == 0) {
            n_t >>= 1;
        } else if ((m & (seq_t)1) == 0) {
            m >>= 1;
        } else {
            /* t must be even */
            t >>= 1;
        }
    }

    return n_t * m * t;
}
