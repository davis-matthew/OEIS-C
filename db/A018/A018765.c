/*
 * A018765  Divisors of 996.
 * https://oeis.org/A018765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th smallest positive divisor of 996 (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of the 12 divisors of 996 */
    static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized) {
        /* Prime factorisation 996 = 2² × 3 × 83 */
        static const uint64_t primes[3] = { 2ULL, 3ULL, 83ULL };
        static const unsigned   exps[3]  = { 2, 1, 1 };

        size_t m = 0;
        for (unsigned i = 0; i <= exps[0]; ++i) {
            seq_t val_i = 1;
            for (unsigned ii = 0; ii < i; ++ii)
                val_i *= (seq_t)primes[0];

            for (unsigned j = 0; j <= exps[1]; ++j) {
                seq_t val_j = val_i;
                for (unsigned jj = 0; jj < j; ++jj)
                    val_j *= (seq_t)primes[1];

                for (unsigned k = 0; k <= exps[2]; ++k) {
                    seq_t val = val_j;
                    for (unsigned kk = 0; kk < k; ++kk)
                        val *= (seq_t)primes[2];
                    divisors[m++] = val;
                }
            }
        }

        /* sort divisors in ascending order (insertion‑sort, trivial for 12 items) */
        for (size_t p = 0; p < m; ++p)
            for (size_t q = p + 1; q < m; ++q)
                if (divisors[q] < divisors[p]) {
                    seq_t tmp = divisors[p];
                    divisors[p] = divisors[q];
                    divisors[q] = tmp;
                }

        initialized = true;
    }

    if (n == 0 || (idx_t)12 < n)
        return 0;                       /* out of bounds */
    return divisors[(size_t)(n - 1)];
}
