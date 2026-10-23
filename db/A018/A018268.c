/*
 * A018268  Divisors of 66.
 * https://oeis.org/A018268
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

/* Divisors of 66 in ascending order */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[8];
    static int initialized = 0;

    if (!initialized) {
        const seq_t primes[3] = {(seq_t)2, (seq_t)3, (seq_t)11};
        size_t idx = 0;
        for (int i = 0; i <= 1; ++i) {
            for (int j = 0; j <= 1; ++j) {
                for (int k = 0; k <= 1; ++k) {
                    seq_t v = (seq_t)1;
                    if (i) v *= primes[0];
                    if (j) v *= primes[1];
                    if (k) v *= primes[2];
                    divs[idx++] = v;
                }
            }
        }
        /* sort ascending */
        for (size_t i = 1; i < 8; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        initialized = 1;
    }

    if (n >= 1 && n <= 8)
        return divs[n - 1];
    return (seq_t)0;
}
