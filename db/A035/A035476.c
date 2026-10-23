/*
 * A035476  Coordination sequence for lattice D*_14 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum(2^k*binomial(14, k)*binomial(n-1, k-1), k=1..14) * +
 *            2^14*binomial(n+6, 13) (for n > 0), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 77, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    static const idx_t N = 14;
    static seq_t C14[15];
    static seq_t pow2[15];
    static int init = 0;

    if (!init) {
        for (idx_t k = 0; k <= N; ++k) {
            C14[k] = binomial(N, k);
            pow2[k] = (seq_t)1 << k;
        }
        init = 1;
    }

    seq_t sum = (seq_t)0;
    idx_t m = n;

    /* first sum: k = 1 .. 14  (k = 0 contributes 0) */
    for (idx_t k = 1; k <= N; ++k) {
        if (k - 1 > m - 1) /* binomial(m-1, k-1) would be 0 */
            break;
        sum += pow2[k] * C14[k] * binomial(m - 1, k - 1);
    }

    /* second term: 2^14 * binomial(n + 6, 13) */
    sum += ((seq_t)1 << 14) * binomial(m + 6, 13);

    return sum;
}
