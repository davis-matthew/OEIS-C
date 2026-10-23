/*
 * A018561  Divisors of 636.
 * https://oeis.org/A018561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 636 in ascending order
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static array of all divisors of 636, computed once */
    static seq_t divisors[12];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* 636 = 2^2 * 3^1 * 53^1, total 12 divisors */
        const idx_t N = (idx_t)636;
        seq_t tmp[24];
        size_t tmpcnt = 0;

        for (idx_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                tmp[tmpcnt++] = (seq_t)i;
                seq_t other = (seq_t)(N / i);
                if (other != (seq_t)i)
                    tmp[tmpcnt++] = other;
            }
        }

        /* sort the collected divisors (smallest first) */
        for (size_t i = 0; i < tmpcnt; ++i) {
            for (size_t j = i + 1; j < tmpcnt; ++j) {
                if (tmp[j] < tmp[i]) {
                    seq_t t = tmp[i];
                    tmp[i] = tmp[j];
                    tmp[j] = t;
                }
            }
        }

        /* copy to the static array */
        for (size_t i = 0; i < tmpcnt; ++i)
            divisors[i] = tmp[i];
        count = tmpcnt;
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    if (n <= count)
        return divisors[(size_t)(n - 1)];
    else
        return (seq_t)0;   /* out of range */
}
