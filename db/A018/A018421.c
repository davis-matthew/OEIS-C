/*
 * A018421  Divisors of 375.
 * https://oeis.org/A018421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Divisors of 375 (A018421). 
   The sequence lists the divisors of 375 in ascending order.
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 375 = 3 * 5^3, so its divisors are 3^e3 * 5^e5 with e3∈{0,1}, e5∈{0,1,2,3} */
    static seq_t divisors[8];
    static int init_done = 0;

    if (!init_done) {
        /* pre‑compute all 8 divisors */
        size_t pos = 0;
        for (idx_t e3 = 0; e3 <= 1; ++e3) {
            seq_t pow3 = 1;
            for (idx_t i = 0; i < e3; ++i) pow3 *= 3;
            for (idx_t e5 = 0; e5 <= 3; ++e5) {
                seq_t pow5 = 1;
                for (idx_t i = 0; i < e5; ++i) pow5 *= 5;
                divisors[pos++] = pow3 * pow5;
            }
        }
        /* sort the 8 elements */
        for (size_t i = 1; i < 8; ++i) {
            size_t j = i;
            while (j > 0 && divisors[j-1] > divisors[j]) {
                seq_t tmp = divisors[j];
                divisors[j] = divisors[j-1];
                divisors[j-1] = tmp;
                --j;
            }
        }
        init_done = 1;
    }

    if (n == 0 || n > 8) {
        return (seq_t)0;          /* out of bounds */
    }
    return divisors[n-1];
}
