/*
 * A018651  Divisors of 796.
 * https://oeis.org/A018651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = list of all positive divisors of 796 sorted in increasing
 *            order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static bool ready = false;

    if (!ready) {
        const seq_t primes[2] = {(seq_t)2, (seq_t)199};
        const idx_t exp_limit[2] = {(idx_t)2, (idx_t)1};

        idx_t idx = 0;
        for (idx_t i = 0; i <= exp_limit[0]; ++i) {
            for (idx_t j = 0; j <= exp_limit[1]; ++j) {
                seq_t val = (seq_t)1;
                for (idx_t k = 0; k < i; ++k) val *= primes[0];
                for (idx_t k = 0; k < j; ++k) val *= primes[1];
                divisors[idx++] = val;
            }
        }
        /* sort the 6 divisors */
        for (idx_t i = 0; i < 6; ++i) {
            for (idx_t j = i + 1; j < 6; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        ready = true;
    }

    if (n < 1 || n > 6)
        return (seq_t)0;
    return divisors[n - 1];
}
