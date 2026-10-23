/*
 * A161459  Number of reduced words of length n in the Weyl group A_11.
 * https://oeis.org/A161459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in Prod_{k=1}^{11} (1 - x^(k+1)) / (1
 *            - x)^11 This is equivalent to a(n) = Σ_{S⊆{2,…,12}} (-1)^|S| ·
 *            binomial(n - ΣS + 10, 10), where binomial(m, k) = 0 if m < k
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t used in loop counters */

/* Local multiplicative binomial coefficient.
   Returns 0 if k > n.  Works for arbitrary-precision seq_t. */
static seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* use symmetry to minimize work */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n - k + i) */
        res = res * (seq_t)(n - k + i);
        /* Exact division by i */
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Offsets of the exponents in the numerator: 2,3,…,12 */
    const uint8_t offset[11] = {2,3,4,5,6,7,8,9,10,11,12};
    const idx_t denom = 10;      /* power of (1-x) in the denominator */

    seq_t posSum = 0;
    seq_t negSum = 0;

    /* Enumerate all subsets of {2,…,12} (11 elements) */
    for (uint64_t mask = 0; mask < (1ULL << 11); ++mask) {
        uint64_t sumIdx = 0;   /* Σ of selected offsets */
        int bits = 0;          /* parity of subset size */
        uint64_t tmp = mask;
        uint8_t i = 0;
        while (tmp) {
            if (tmp & 1) {
                sumIdx += offset[i];
                ++bits;
            }
            ++i;
            tmp >>= 1;
        }

        if (n < sumIdx) continue;                 /* coefficient is 0 */

        idx_t top = n - sumIdx + denom;           /* n - ΣS + 10 */
        seq_t term = binom_local(top, denom);     /* binomial(top,10) */

        if (bits & 1)
            negSum += term;
        else
            posSum += term;
    }

    return posSum - negSum;
}
