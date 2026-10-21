/*
 * A088379  a(n) = (smallest prime factor of n)^4; a(1) = 1.
 * https://oeis.org/A088379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (smallest prime factor of n)^4; a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle offset: a(1) = 1 */
    if (n <= 1) {
        return (seq_t)1;
    }

    uint64_t x = (uint64_t)n;
    uint64_t spf = 0;

    /* Find smallest prime factor */
    if ((x & 1ULL) == 0) {
        spf = 2;
    } else {
        uint64_t i = 3;
        uint64_t lim = (uint64_t)(x / (i * i));
        /* Check odd divisors up to sqrt(x) */
        while (i * i <= x) {
            if (x % i == 0) {
                spf = i;
                break;
            }
            i += 2;
        }
        if (spf == 0) { /* x itself is prime */
            spf = x;
        }
    }

    seq_t p = (seq_t)spf;
    seq_t vp2 = p * p;   /* p^2 */
    seq_t result = vp2 * vp2; /* p^4 */
    return result;
}
