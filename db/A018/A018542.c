/*
 * A018542  Divisors of 602.
 * https://oeis.org/A018542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 602 in increasing order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Produces the n-th divisor of 602 (1-indexed).  For n>8 the function
   returns 0, as 602 has exactly 8 positive divisors. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* The prime factorisation of 602 is 2 * 7 * 43, all with exponent 1.
       Therefore there are 2^3 = 8 divisors. */
    seq_t divs[8];
    int idx = 0;
    for (uint8_t e2 = 0; e2 <= 1; ++e2) {
        for (uint8_t e7 = 0; e7 <= 1; ++e7) {
            for (uint8_t e43 = 0; e43 <= 1; ++e43) {
                seq_t d = 1;
                if (e2)  d = d * 2;
                if (e7)  d = d * 7;
                if (e43) d = d * 43;
                divs[idx++] = d;
            }
        }
    }

    /* Sort the array of divisors in ascending order. */
    for (int i = 0; i < 7; ++i) {
        for (int j = i + 1; j < 8; ++j) {
            if (divs[j] < divs[i]) {
                seq_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
        }
    }

    if (n > 8) return 0;   /* 602 has only 8 positive divisors. */

    return divs[n - 1];
}
