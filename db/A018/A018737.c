/*
 * A018737  Divisors of 946.
 * https://oeis.org/A018737
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

/* Sequence: divisors of 946 in ascending order. */

#include <stddef.h>

/* Return the nth divisor of 946, where n >= 1.
   For n > 8, or n == 0, the function returns 0. */
seq_t a(idx_t n)
{
    /* Initialise the divisor array only once. */
    static seq_t divs[8];
    static int init = 0;

    if (!init) {
        seq_t val;
        const int primes[3] = {2, 11, 43};
        int cnt = 0;

        /* Generate all products of subsets of the primes. */
        for (int mask = 0; mask < 8; ++mask) {
            val = 1;
            if (mask & 1) val *= (seq_t)primes[0];
            if (mask & 2) val *= (seq_t)primes[1];
            if (mask & 4) val *= (seq_t)primes[2];
            divs[cnt++] = val;
        }

        /* Sort the divisors in ascending order. */
        for (int i = 0; i < 7; ++i) {
            for (int j = i + 1; j < 8; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        init = 1;
    }

    if (n == 0 || n > 8) {
        return (seq_t)0;
    }
    return divs[(size_t)(n - 1)];
}
