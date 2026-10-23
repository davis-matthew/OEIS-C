/*
 * A018320  Divisors of 176.
 * https://oeis.org/A018320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 176, obtained by enumerating the exponents of its prime
   factorisation 176 = 2^4 * 11^1 and sorting the results. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[10];
    static size_t div_cnt = 0;

    if (!initialized) {
        /* Generate all divisors from the factorisation */
        seq_t tmp[10];
        size_t pos = 0;

        for (int a = 0; a <= 4; ++a) {      /* exponent of 2 */
            for (int b = 0; b <= 1; ++b) {  /* exponent of 11 */
                seq_t value = (seq_t)((uint64_t)1 << a);
                if (b)
                    value *= (seq_t)11;
                tmp[pos++] = value;
            }
        }
        div_cnt = pos;

        /* Sort the array in ascending order (insertion sort). */
        for (size_t i = 1; i < div_cnt; ++i) {
            seq_t key = tmp[i];
            size_t j = i;
            while (j > 0 && tmp[j - 1] > key) {
                tmp[j] = tmp[j - 1];
                --j;
            }
            tmp[j] = key;
        }

        /* Cache the sorted divisors. */
        for (size_t i = 0; i < div_cnt; ++i)
            divisors[i] = tmp[i];

        initialized = true;
    }

    if (n == 0 || n > div_cnt)
        return (seq_t)0;   /* undefined for indices outside the range */

    return divisors[(size_t)(n - 1)];
}
