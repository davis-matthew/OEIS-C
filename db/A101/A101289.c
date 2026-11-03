/*
 * A101289  Inverse Moebius transform of 5-simplex numbers A000389.
 * https://oeis.org/A101289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} binomial(d+4,5)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 217, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9762 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial(d+4,5) using a multiplicative scheme that stays within
 * the range of seq_t for all n that fit the output.
 * The intermediate product is cast to unsigned __int128 to avoid overflow
 * when seq_t is 64‑bit. The final result is guaranteed to fit in seq_t
 * if the sequence value itself fits.
 */
static seq_t binom_d_plus_4_choose_5(idx_t d) {
    seq_t result = 1;
    for (int k = 1; k <= 5; ++k) {
        /* (d + 5 - k) is the current numerator */
        idx_t numer = d + 5 - k;
        unsigned __int128 prod = (unsigned __int128)result * numer;
        result = (seq_t)(prod / k);  /* division is exact */
    }
    return result;
}

/* Main function required by the build.  It uses only integer arithmetic
 * on seq_t and idx_t and does not depend on any external utilities.
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* safety guard – not part of sequence */

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            sum += binom_d_plus_4_choose_5(i);
            idx_t j = n / i;
            if (j != i) {
                /* complementary divisor j */
                sum += binom_d_plus_4_choose_5(j);
            }
        }
    }
    return sum;
}
