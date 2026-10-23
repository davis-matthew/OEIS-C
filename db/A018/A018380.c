/*
 * A018380  Divisors of 296.
 * https://oeis.org/A018380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 296 in increasing order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Number of divisors of 296 is known to be 8 */
    static bool init = false;
    static seq_t divisors[8];
    static size_t dcnt = 0;

    if (!init) {
        const idx_t N = 296;
        size_t pos = 0;
        for (idx_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                if (pos < sizeof(divisors)/sizeof(divisors[0]))
                    divisors[pos++] = (seq_t)i;
                idx_t j = N / i;
                if (j != i && pos < sizeof(divisors)/sizeof(divisors[0]))
                    divisors[pos++] = (seq_t)j;
            }
        }
        /* sort divisors in increasing order (simple insertion sort) */
        for (size_t i = 1; i < pos; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        dcnt = pos;
        init = true;
    }

    if (n >= 1 && n <= dcnt)
        return divisors[(size_t)(n - 1)];
    return 0;
}
