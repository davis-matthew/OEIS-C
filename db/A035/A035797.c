/*
 * A035797  Coordination sequence for lattice D*_24 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(m) = Sum_{k=0..24} 2^k*binomial(24,k)*binomial(m-1,k-1) * +
 *            2^24*binomial(m+11,23)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset term a(0) */
    if (n == 0)
        return (seq_t)1;

    /* Lazily initialize lookup tables */
    static bool initialized = false;
    static seq_t pow2k[25];
    static seq_t binom24[25];
    static seq_t pow2_24;

    if (!initialized) {
        pow2k[0] = pow2(0);
        for (idx_t k = 1; k <= 24; ++k)
            pow2k[k] = pow2(k);
        for (idx_t k = 0; k <= 24; ++k)
            binom24[k] = binomial(24, k);
        pow2_24 = pow2k[24];
        initialized = true;
    }

    seq_t sum = (seq_t)0;

    /* Sum_{k=1..24} 2^k*binomial(24,k)*binomial(n-1,k-1) */
    for (idx_t k = 1; k <= 24; ++k) {
        seq_t term1 = pow2k[k] * binom24[k];
        seq_t term2 = binomial((idx_t)(n - 1), k - 1);
        sum += term1 * term2;
    }

    /* Add the 2^24*binomial(n+11,23) term */
    seq_t term_last = pow2_24 * binomial((idx_t)(n + 11), 23);
    sum += term_last;

    return sum;
}
