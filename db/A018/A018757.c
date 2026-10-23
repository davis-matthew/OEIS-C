/*
 * A018757  Divisors of 981.
 * https://oeis.org/A018757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 981 in ascending order
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache divisors of 981, computed only once */
    static seq_t divs[128];
    static size_t cnt = 0;

    if (cnt == 0) {
        const seq_t N = (seq_t)981;
        for (idx_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                divs[cnt++] = (seq_t)i;
                if (i * i != N) {
                    divs[cnt++] = N / i;
                }
            }
        }
        /* simple selection sort to order ascending */
        for (size_t i = 0; i < cnt; ++i) {
            for (size_t j = i + 1; j < cnt; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
    }

    if (n == 0 || n > cnt)
        return 0;

    return divs[(size_t)(n - 1)];
}
