/*
 * A182421  a(n) = Sum_{k = 0..n} C(n,k)^7.
 * https://oeis.org/A182421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;            /* C(n,0) */

    /* k = 0 term : 1^7 = 1 */
    sum += 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* C(n,k) = C(n,k-1) * (n-k+1) / k */
        bin = (bin * (seq_t)(n - k + 1)) / (seq_t)k;

        /* Compute bin^7 by repeated multiplication */
        seq_t pow7 = bin;
        for (int i = 1; i < 7; ++i) pow7 *= bin;

        sum += pow7;
    }

    return sum;
}
