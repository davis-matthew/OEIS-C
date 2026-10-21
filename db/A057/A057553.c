/*
 * A057553  Rank of (1,1,...,1) (n 1's) when {0,1,2,...}^n is
 *          lexicographically ordered.
 * https://oeis.org/A057553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n-1, n) + sum_{i=1}^{n-1} C(2i, i-1) + 1 (a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used, offset starts at 1 */
    if (n == 1) return 1;

    /* Compute D = C(2n-1, n) using the recurrence:
       D(1) = 1, D(k+1) = D(k) * (2k+1)(2k) / [k(k+1)]          */
    seq_t D = 1;
    for (idx_t k = 1; k < n; ++k) {
        seq_t num = (seq_t)(2 * k + 1) * (seq_t)(2 * k);
        seq_t den = (seq_t)k * (seq_t)(k + 1);
        D = D * num / den;
    }

    /* Compute L = sum_{i=1}^{n-1} C(2i, i-1) using the recurrence:
       B(1) = C(2,0) = 1, B(i+1) = B(i) * (2i+2)(2i+1) / [i(i+2)]  */
    seq_t L = 0;
    seq_t B = 1;          /* B(1) */
    L += B;
    for (idx_t i = 1; i < n - 1; ++i) {
        seq_t num = (seq_t)(2 * i + 2) * (seq_t)(2 * i + 1);
        seq_t den = (seq_t)i * (seq_t)(i + 2);
        B = B * num / den;
        L += B;
    }

    return D + L + 1;
}
