/*
 * A151779  a(1)=1; for n > 1, a(n)=6*5^{wt(n-1)-1}.
 * https://oeis.org/A151779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=1); a(n) = 6*5^{wt(n-1)-1} (n>1)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* wt is the number of 1 bits in the binary representation of n-1 */

static inline unsigned int popcount_u64(idx_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }

    /* compute popcount of n-1 */
    unsigned int k = popcount_u64(n - 1); /* k >= 1 for n > 1 */

    seq_t result = 6;          /* start with the factor 6 */
    seq_t pow5   = 1;          /* 5^{k-1} */

    /* multiply pow5 by 5, (k-1) times */
    for (unsigned int i = 0; i < k - 1; ++i) {
        pow5 = pow5 * 5;
    }

    result = result * pow5;
    return result;
}
