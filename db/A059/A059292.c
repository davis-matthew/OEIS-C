/*
 * A059292  a(n) = n + 2 - (number of divisors of n).
 * https://oeis.org/A059292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2 - d(n) (d(n) = number of divisors of n)
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Count divisors of an unsigned 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t cnt = 1;
    uint64_t m = n;

    /* factor 2 first */
    if ((m & 1ULL) == 0) {
        unsigned exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++exp;
        }
        cnt *= (exp + 1);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }

    if (m > 1) cnt *= 2;          /* remaining prime factor */
    return cnt;
}

/* OEIS sequence A059292 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: undefined for n < 1 */

    uint64_t tau = divisor_count_u64((uint64_t)n);
    seq_t val = (seq_t)n + 2 - (seq_t)tau;
    return val;
}
