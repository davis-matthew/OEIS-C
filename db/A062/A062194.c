/*
 * A062194  Fifth column sequence of triangle A062139 (generalized a=2
 *          Laguerre).
 * https://oeis.org/A062194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)! * binomial(n+6,6) / 24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation cancels the 17280 = 2^7·3^3·5 denominator incrementally
   while multiplying the numbers 1..n+4 and n+1..n+6, keeping the intermediate
   result within the final value for all supported seq_t sizes. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* prime factors of the denominator 17280 = 2^7·3^3·5 */
    static const idx_t primes[3] = {2, 3, 5};
    int exp[3] = {7, 3, 1};

    seq_t result = (seq_t)1;

    /* multiply numbers 1 .. n+4 */
    for (idx_t i = 1; i <= n + 4; ++i) {
        seq_t val = (seq_t)i;
        for (int p = 0; p < 3; ++p) {
            while (exp[p] > 0 && (val % primes[p]) == 0) {
                val /= primes[p];
                exp[p]--;
            }
        }
        result *= val;
    }

    /* multiply numbers n+1 .. n+6 (six numbers from n+1 to n+6) */
    for (idx_t i = n + 1; i <= n + 6; ++i) {
        seq_t val = (seq_t)i;
        for (int p = 0; p < 3; ++p) {
            while (exp[p] > 0 && (val % primes[p]) == 0) {
                val /= primes[p];
                exp[p]--;
            }
        }
        result *= val;
    }

    /* any remaining denominator factors (should be none) are divided */
    for (int p = 0; p < 3; ++p) {
        while (exp[p] > 0) {
            result /= primes[p];
            exp[p]--;
        }
    }

    return result;
}
