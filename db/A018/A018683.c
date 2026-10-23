/*
 * A018683  Divisors of 852.
 * https://oeis.org/A018683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 852 sorted in ascending order */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t divisors[12];
    static bool init = false;
    if (!init) {
        /* prime factorization 852 = 2^2 * 3^1 * 71^1 */
        const seq_t bases[3] = {2, 3, 71};
        const unsigned exp[3] = {2, 1, 1};
        size_t count = 0;

        for (unsigned e0 = 0; e0 <= exp[0]; ++e0) {
            seq_t d0 = 1;
            for (unsigned i = 0; i < e0; ++i) d0 *= bases[0];
            for (unsigned e1 = 0; e1 <= exp[1]; ++e1) {
                seq_t d1 = d0;
                for (unsigned i = 0; i < e1; ++i) d1 *= bases[1];
                for (unsigned e2 = 0; e2 <= exp[2]; ++e2) {
                    seq_t d2 = d1;
                    for (unsigned i = 0; i < e2; ++i) d2 *= bases[2];
                    divisors[count++] = d2;
                }
            }
        }

        /* simple selection sort, list is tiny */
        for (size_t i = 0; i < count; ++i) {
            size_t min = i;
            for (size_t j = i + 1; j < count; ++j) {
                if (divisors[j] < divisors[min]) min = j;
            }
            if (min != i) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[min];
                divisors[min] = tmp;
            }
        }
        init = true;
    }

    if (n < 1) return 0;
    if (n - 1 >= 12) return 0;   /* no further divisors */
    return divisors[n - 1];
}
