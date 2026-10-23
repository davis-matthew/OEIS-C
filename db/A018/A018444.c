/*
 * A018444  Divisors of 420.
 * https://oeis.org/A018444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th divisor of 420 in increasing order */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The number 420 = 2^2 · 3 · 5 · 7 has (2+1)(1+1)(1+1)(1+1) = 24 divisors.
       We generate them once, sort, and store them in a static array. */
    static seq_t divisors[24];
    static bool ready = false;
    static size_t count = 0;

    if (!ready) {
        /* Generate all divisors of 420 */
        uint64_t tmp[24];
        size_t idx = 0;
        for (int a = 0; a <= 2; ++a) {
            for (int b = 0; b <= 1; ++b) {
                for (int c = 0; c <= 1; ++c) {
                    for (int d = 0; d <= 1; ++d) {
                        uint64_t val = (1ULL << a);          /* 2^a */
                        val *= (b ? 3ULL : 1ULL);            /* *3^b */
                        val *= (c ? 5ULL : 1ULL);            /* *5^c */
                        val *= (d ? 7ULL : 1ULL);            /* *7^d */
                        tmp[idx++] = val;
                    }
                }
            }
        }
        /* tmp now contains 24 values; sort them. */
        for (size_t i = 1; i < idx; ++i) {
            seq_t key = (seq_t)tmp[i];
            size_t j = i;
            while (j > 0 && (seq_t)tmp[j - 1] > key) {
                tmp[j] = tmp[j - 1];
                --j;
            }
            tmp[j] = (uint64_t)key;
        }
        /* Copy into the static array. */
        for (size_t i = 0; i < idx; ++i) {
            divisors[i] = (seq_t)tmp[i];
        }
        count = idx;
        ready = true;
    }

    if (n < 1 || n > count) {
        return (seq_t)0;
    }
    return divisors[n - 1];
}
