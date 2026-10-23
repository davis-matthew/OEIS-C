/*
 * A018442  Divisors of 416.
 * https://oeis.org/A018442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 416 in increasing order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static data for the 12 divisors of 416 */
    static bool initialized = false;
    static seq_t divs[12];
    static size_t div_len = 0;

    if (!initialized) {
        /* 416 = 2^5 * 13 */
        const idx_t N = 416;
        idx_t tmp = N;
        idx_t exp2 = 0;
        while (tmp % 2 == 0) {
            exp2++;
            tmp /= 2;
        }
        /* Generate all divisors 2^e2 * 13^e13 */
        size_t idx = 0;
        for (idx_t e2 = 0; e2 <= exp2; ++e2) {
            seq_t val2 = (seq_t)1;
            for (idx_t i = 0; i < e2; ++i) val2 *= (seq_t)2;
            for (idx_t e13 = 0; e13 <= 1; ++e13) {
                seq_t val = val2;
                for (idx_t i = 0; i < e13; ++i) val *= (seq_t)13;
                divs[idx++] = val;
            }
        }
        div_len = idx; /* should be 12 */

        /* insertion sort to get ascending order */
        for (size_t i = 1; i < div_len; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                j--;
            }
            divs[j] = key;
        }
        initialized = true;
    }

    if (n >= 1 && ((idx_t)n - 1) < div_len) {
        return divs[(size_t)n - 1];
    }
    return (seq_t)0; /* undefined for n > 12 */
}
