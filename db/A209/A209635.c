/*
 * A209635  Möbius mu-function applied to the odd part of n: a(n) =
 *          A008683(A000265(n)).
 * https://oeis.org/A209635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign
 * Formula:   a(n) = A008683(A000265(n)) -- Möbius of the odd part of n
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0) odd >>= 1;           /* remove all factors of 2 */

    seq_t res = 1;
    uint64_t temp = odd;

    for (uint64_t p = 3; p <= temp / p; p += 2) {   /* trial division by odd primes */
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt > 1) return 0;             /* squared prime factor => μ = 0 */
            }
            res = -res;                            /* flip sign for each distinct prime */
        }
    }

    if (temp > 1) {                                /* remaining prime factor > 1 */
        res = -res;
    }

    return res;
}
