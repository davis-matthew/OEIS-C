/*
 * A018701  Divisors of 884.
 * https://oeis.org/A018701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 884 in ascending order
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static array of divisors of 884 (2^2 * 13 * 17) */
    static seq_t divs[12];
    static idx_t div_count = 0;
    static bool sorted = false;
    if (div_count == 0) {
        /* generate all combinations of exponents 0..2 for prime 2,
           0..1 for prime 13, 0..1 for prime 17 */
        const seq_t pow2[3]  = { (seq_t)1, (seq_t)2, (seq_t)4 };
        const seq_t pow13[2] = { (seq_t)1, (seq_t)13 };
        const seq_t pow17[2] = { (seq_t)1, (seq_t)17 };
        for (idx_t e0 = 0; e0 <= 2; ++e0) {
            for (idx_t e1 = 0; e1 <= 1; ++e1) {
                for (idx_t e2 = 0; e2 <= 1; ++e2) {
                    divs[div_count++] = pow2[e0] * pow13[e1] * pow17[e2];
                }
            }
        }
    }
    if (!sorted) {
        /* insertion sort to order divisors ascendingly */
        for (idx_t i = 1; i < div_count; ++i) {
            idx_t j = i;
            while (j > 0 && divs[j - 1] > divs[j]) {
                seq_t tmp = divs[j - 1];
                divs[j - 1] = divs[j];
                divs[j] = tmp;
                --j;
            }
        }
        sorted = true;
    }
    if (n >= 1 && n <= div_count) {
        return divs[(size_t)(n - 1)];
    }
    /* undefined for n beyond the number of divisors */
    return 0;
}
