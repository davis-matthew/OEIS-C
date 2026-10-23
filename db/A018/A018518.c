/*
 * A018518  Divisors of 558.
 * https://oeis.org/A018518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 558 in increasing order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t divisors[24];
static size_t divisor_count = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        const idx_t LIMIT = (idx_t)558;
        size_t cnt = 0;

        /* Find all divisors of LIMIT */
        for (idx_t i = 1; i * i <= LIMIT; ++i) {
            if (LIMIT % i == 0) {
                divisors[cnt++] = (seq_t)i;
                idx_t j = LIMIT / i;
                if (j != i) {
                    divisors[cnt++] = (seq_t)j;
                }
            }
        }

        /* Simple bubble sort to order the divisors */
        for (size_t i = 0; i < cnt; ++i) {
            for (size_t j = i + 1; j < cnt; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        divisor_count = cnt;
        initialized = 1;
    }

    if (n == 0 || n > (idx_t)divisor_count) {
        /* For indices out of range return 0 */
        return (seq_t)0;
    }
    return divisors[(size_t)(n - 1)];
}
