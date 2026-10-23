/*
 * A018452  Divisors of 432.
 * https://oeis.org/A018452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 432 in ascending order; 0 for n > 20
 * Range:     u8 n <= 18, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divs[20];
    static bool init = false;
    if (!init) {
        /* Compute all divisors of 432 */
        seq_t val432 = (seq_t)432;
        idx_t count = 0;
        /* iterate over i such that i*i <= 432 */
        for (idx_t i = 1; i * i <= 432; ++i) {
            if (432 % i == 0) {
                divs[count++] = (seq_t)i;
                idx_t j = 432 / i;
                if (j != i) {
                    divs[count++] = (seq_t)j;
                }
            }
        }
        /* sort the divisors ascending (count <= 20) */
        for (idx_t i = 0; i < count; ++i) {
            for (idx_t j = i + 1; j < count; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        init = true;
    }

    if (n == 0) {
        return (seq_t)0; /* undefined for n < 1 */
    }
    idx_t idx = n - 1;
    if (idx < 20) {
        return divs[idx];
    } else {
        return (seq_t)0; /* undefined for n > 20 */
    }
}
