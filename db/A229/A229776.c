/*
 * A229776  Number of 4X4 0..n arrays with rows in lexicographically
 *          nondecreasing order and columns in nonincreasing order.
 * https://oeis.org/A229776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{16} c_k * binom(n, k) (binomial basis)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients c_k are the ordinary finite differences of the */
/* sequence a(n) at n = 0; they are integers.  */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Coefficients c_0 … c_16 obtained from the first 17 terms
       a(0) … a(16).  They are used in the binomial basis. */
    static seq_t coeff[17];
    static bool initialized = false;
    if (!initialized) {
        /* First 17 terms of the sequence (a(0) = 1, a(1) = 70, …) */
        const unsigned long long f[17] = {
            1ULL,
            70ULL,
            12958ULL,
            1058494ULL,
            35452250ULL,
            637396928ULL,
            7379634620ULL,
            61734072288ULL,
            402537583863ULL,
            2155573868470ULL,
            9841145888530ULL,
            39380724254950ULL,
            141067066333648ULL,
            459822149867432ULL,
            1381730749348760ULL,
            3867950992777232ULL,
            10173948824534685ULL
        };

        unsigned long long diff[17];
        for (idx_t i = 0; i <= 16; ++i) diff[i] = f[i];
        for (idx_t k = 0; k <= 16; ++k) {
            coeff[k] = (seq_t)diff[0];
            for (idx_t i = 0; i < 16 - k; ++i) {
                diff[i] = diff[i + 1] - diff[i];
            }
        }
        initialized = true;
    }

    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= 16; ++k) {
        /* Compute binomial coefficient binom(n, k) */
        seq_t bin = (seq_t)1;
        for (idx_t i = 1; i <= k; ++i) {
            bin = (bin * (seq_t)(n - k + i)) / (seq_t)i;
        }
        result += coeff[k] * bin;
    }
    return result;
}
