/*
 * A018723  Divisors of 924.
 * https://oeis.org/A018723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Divisors of 924: a(n) = the n‑th divisor of 924 in increasing order. */
seq_t a(idx_t n)
{
    /* cache of the 24 sorted divisors of 924 */
    static seq_t divisors[24];
    static size_t num_div = 0;
    static bool initialized = false;

    const idx_t N = 924;

    if (!initialized) {
        /* temporary array to collect divisors (unordered) */
        seq_t temp[48];
        size_t cnt = 0;

        for (idx_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                temp[cnt++] = (seq_t)i;
                idx_t j = N / i;
                if (j != i) {
                    temp[cnt++] = (seq_t)j;
                }
            }
        }

        /* sort the collected divisors in ascending order */
        for (size_t i = 0; i < cnt; ++i) {
            for (size_t j = i + 1; j < cnt; ++j) {
                if (temp[j] < temp[i]) {
                    seq_t t = temp[i];
                    temp[i] = temp[j];
                    temp[j] = t;
                }
            }
        }

        /* copy the sorted divisors into the static cache */
        for (size_t i = 0; i < cnt; ++i) {
            divisors[i] = temp[i];
        }
        num_div = cnt;
        initialized = true;
    }

    if (n == 0 || n > num_div) {
        return (seq_t)0;
    }
    return divisors[(size_t)(n - 1)];
}
