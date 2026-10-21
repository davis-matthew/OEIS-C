/*
 * A066660  Number of divisors of 2n excluding 1.
 * https://oeis.org/A066660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(2n) - 1, d(m) = number of divisors of m. We factor n =
 *            2^e * m (m odd). Then d(2n) = (e + 2) * d(m), so a(n) = (e + 2)
 *            * d(m) - 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 for the defined offset (a(1) is the first term).  */
    if (n == 0) return (seq_t)0;   /* undefined; return 0 for safety */

    /* Count the exponent of 2 in n. */
    uint64_t e = 0;
    uint64_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e;
    }

    /* Count divisors of the odd part m = tmp. */
    uint64_t prod = 1;
    uint64_t t = tmp;
    /* Try odd divisors only, up to sqrt(t). */
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            prod *= (exp + 1);
        }
    }
    if (t > 1) {
        /* Remaining prime factor. */
        prod *= 2;
    }

    /* d(2n) = (e + 2) * prod.  Subtract 1 to exclude divisor 1. */
    uint64_t result = (e + 2) * prod - 1;

    return (seq_t)result;
}
