/*
 * A067443  Smallest n-th power starting with 2.
 * https://oeis.org/A067443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = min{k^n | k>=2, first digit of k^n is 2}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for offset 1, but return 0 */
    /* Logarithmic bounds for leading digit 2 */
    const double LO = log10(2.0);
    const double HI = log10(3.0);
    const double EPS = 1e-12;
    for (uint64_t k = 2;; ++k) {
        double val = (double)n * log10((double)k);
        double frac = val - floor(val);
        if (frac >= LO - EPS && frac < HI - EPS) {
            return big_pow((seq_t)k, (seq_t)n);
        }
    }
}
