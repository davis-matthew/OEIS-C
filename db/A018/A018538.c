/*
 * A018538  Divisors of 595.
 * https://oeis.org/A018538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 595 in increasing order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1 */

    static seq_t divisors[8];
    static bool initialized = false;

    if (!initialized) {
        /* factor 595 */
        idx_t m = 595;
        idx_t primes[3];
        size_t pcount = 0;
        for (idx_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                primes[pcount++] = p;
                while (m % p == 0) m /= p;
            }
        }
        if (m > 1 && pcount < 3)
            primes[pcount++] = m;

        /* generate all divisors */
        seq_t divs[8];
        size_t count = 1;   /* start with divisor 1 */
        divs[0] = 1;
        for (size_t i = 0; i < pcount; ++i) {
            idx_t p = primes[i];
            size_t cur = count;
            for (size_t j = 0; j < cur; ++j)
                divs[count + j] = divs[j] * (seq_t)p;
            count += cur;
        }

        /* sort the divisors */
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }

        for (size_t i = 0; i < count; ++i)
            divisors[i] = divs[i];
        initialized = true;
    }

    if (n <= 8) return divisors[n - 1];
    return 0;
}
