/*
 * A166633  Totally multiplicative sequence with a(p) = 3*(p-1) for prime p.
 * https://oeis.org/A166633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of (3*(p-1))^e
 * Range:     u8 n <= 35, u16 n <= 971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Totally multiplicative with a(p) = 3*(p-1)) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Handle factor 2 separately for speed */
    while ((m & 1ULL) == 0) {     /* m % 2 == 0 */
        result *= (seq_t)(3 * (2 - 1));  /* term for prime 2 */
        m >>= 1;                 /* m /= 2 */
    }

    /* Check odd factors up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            result *= (seq_t)(3 * (p - 1));  /* term for prime p */
            m /= p;
        }
    }

    /* If m > 1 now, it's a remaining prime factor */
    if (m > 1)
        result *= (seq_t)(3 * (m - 1));

    return result;
}
