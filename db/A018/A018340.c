/*
 * A018340  Divisors of 222.
 * https://oeis.org/A018340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth smallest divisor of 222 in
   increasing order.  For n > 8 the value 0 is returned. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n <= 0 */

    /* the prime factorisation of 222 is 2^1 * 3^1 * 37^1 */
    static seq_t divisors[8];
    static bool init = false;

    if (!init) {
        size_t idx = 0;
        for (int e2 = 0; e2 <= 1; ++e2) {
            for (int e3 = 0; e3 <= 1; ++e3) {
                for (int e37 = 0; e37 <= 1; ++e37) {
                    seq_t val = 1;
                    for (int i = 0; i < e2; ++i) val *= 2;
                    for (int i = 0; i < e3; ++i) val *= 3;
                    for (int i = 0; i < e37; ++i) val *= 37;
                    divisors[idx++] = val;
                }
            }
        }
        /* insertion sort – the array is tiny */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        init = true;
    }

    if (n > 8) return (seq_t)0;
    return divisors[(size_t)(n - 1)];
}
