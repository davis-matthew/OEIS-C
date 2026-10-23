/*
 * A018526  Divisors of 574.
 * https://oeis.org/A018526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 574 (sorted increasingly)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Divisors of 574 = 2 * 7 * 41.  There are 8 of them. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* Prime factorisation of 574. */
    const uint8_t primes[3] = {2, 7, 41};
    const uint8_t exps[3]   = {1, 1, 1};

    seq_t divs[8];
    int count = 0;
    divs[count++] = 1;                /* start with 1 */

    for (uint8_t i = 0; i < 3; ++i) {
        uint8_t p = primes[i];
        int pre = count;              /* number of divisors so far */
        for (uint8_t e = 1; e <= exps[i]; ++e) {
            seq_t mult = 1;
            for (uint8_t k = 0; k < e; ++k)
                mult *= p;
            for (int j = 0; j < pre; ++j)
                divs[count++] = divs[j] * mult;
        }
    }

    /* Simple insertion sort (max 8 items). */
    for (int i = 1; i < count; ++i) {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    if (n <= (idx_t)count)
        return divs[(size_t)n - 1];
    return 0;     /* undefined indices return 0 */
}
