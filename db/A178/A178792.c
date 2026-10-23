/*
 * A178792  Dot product of the rows of triangle A046899 with vector
 *          (1,2,4,8,...) (= A000079).
 * https://oeis.org/A178792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} 2^k * binomial(n + k, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t binom = 1;   /* binomial(n + 0, 0) */
    seq_t pow2  = 1;   /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* binom <-- binom * (n + k) / k   (exact division) */
            binom = binom * (n + k) / k;
            pow2  = pow2 * 2;
        }
        sum += binom * pow2;
    }
    return sum;
}
