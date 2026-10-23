/*
 * A139541  There are 4*n players who wish to play bridge at n tables. Each
 *          player must have another player as partner and each pair of
 *          partners must have another pair as opponents. The choice of
 *          partners and opponents can be made in exactly a(n)=(4*n)!/(n!*8^n)
 *          different ways.
 * https://oeis.org/A139541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)!/(n!*8^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Compute a(n) = (4*n)!/(n!*8^n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t upper = 4 * n;
    size_t size = (size_t)upper + 1;
    uint8_t *sieve = (uint8_t *)calloc(size, 1);
    if (!sieve)
        return 0; /* memory failure */

    for (idx_t i = 2; i * i <= upper; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= upper; j += i)
                sieve[j] = 1;
        }
    }

    seq_t result = 1;

    for (idx_t p = 2; p <= upper; ++p) {
        if (!sieve[p]) {
            long long expo = 0;

            /* exponent of p in (4n)! */
            idx_t m = upper;
            while (m) {
                m /= p;
                expo += m;
            }

            /* subtract exponent of p in n! */
            m = n;
            while (m) {
                m /= p;
                expo -= m;
            }

            /* subtract exponent of p in 8^n (only for p == 2) */
            if (p == 2)
                expo -= 3L * (long long)n;

            if (expo > 0) {
                seq_t val = (seq_t)p;
                for (long long k = 0; k < expo; ++k)
                    result *= val;
            }
        }
    }

    free(sieve);
    return result;
}
