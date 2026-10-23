/*
 * A028949  Write numbers from 1 to n(n+1)/2 in a left-justified lower
 *          triangular array (a) downwards and (b) across; a(n) is number of
 *          numbers in same position in both.
 * https://oeis.org/A028949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n=1, otherwise a(n) = 2 + d(2*n^2 - 6*n + 5)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(k) is the number of positive divisors of k. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    unsigned __int128 tmp = (unsigned __int128)2 * nn * nn
                         - (unsigned __int128)6 * nn
                         + (unsigned __int128)5;
    uint64_t k = (uint64_t)tmp;   /* k > 0 for all n >= 2 */

    seq_t divcnt = 1;
    uint64_t m = k;

    /* Count factor 2 */
    uint32_t exp = 0;
    while ((m & 1ULL) == 0) {
        exp++;
        m >>= 1;
    }
    if (exp > 0)
        divcnt *= (seq_t)(exp + 1);

    /* Count odd prime factors */
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                m /= i;
                exp++;
            }
            divcnt *= (seq_t)(exp + 1);
        }
    }
    if (m > 1)
        divcnt *= (seq_t)2;

    return (seq_t)2 + divcnt;
}
