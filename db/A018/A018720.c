/*
 * A018720  Divisors of 916.
 * https://oeis.org/A018720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 916 in ascending order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[6];
    static size_t div_count = 0;
    static bool init = false;

    if (!init) {
        /* factor 916 = 2^2 * 229 */
        uint64_t tmp = 916;
        uint64_t primes[64];
        int exps[64];
        int pcnt = 0;

        uint64_t f;
        for (f = 2; f * f <= tmp; ++f) {
            if (tmp % f == 0) {
                int e = 0;
                while (tmp % f == 0) {
                    tmp /= f;
                    ++e;
                }
                primes[pcnt] = f;
                exps[pcnt] = e;
                ++pcnt;
            }
        }
        if (tmp > 1) {
            primes[pcnt] = tmp;
            exps[pcnt] = 1;
            ++pcnt;
        }

        /* generate all divisors */
        divs[0] = 1;
        div_count = 1;
        for (int i = 0; i < pcnt; ++i) {
            size_t old_count = div_count;
            uint64_t power = 1;
            for (int k = 1; k <= exps[i]; ++k) {
                power *= primes[i];
                for (size_t j = 0; j < old_count; ++j) {
                    divs[div_count++] = divs[j] * power;
                }
            }
        }
        init = true;
    }

    if (n == 0 || n > div_count) return 0;
    return divs[n - 1];
}
