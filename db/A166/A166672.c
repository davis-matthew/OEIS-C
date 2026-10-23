/*
 * A166672  Totally multiplicative sequence with a(p) = 9p-2 for prime p.
 * https://oeis.org/A166672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} (9p‑2)^e (multiplicative, a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for 0, guard against accidental calls */

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* trivial case */
    if (m == 1) return result;

    /* factor  m using trial division up to sqrt(m) */
    uint64_t i = 2;
    while (i * i <= m) {
        if (m % i == 0) {
            /* count the exponent of prime i */
            uint64_t exp = 0;
            while (m % i == 0) {
                m /= i;
                ++exp;
            }
            /* multiply result by (9*i-2)^exp */
            seq_t base = (seq_t)(9ULL * i - 2ULL);
            while (exp--) result *= base;
        }
        ++i;
    }

    /* if m > 1, remaining factor is a prime */
    if (m > 1) {
        seq_t base = (seq_t)(9ULL * m - 2ULL);
        result *= base;
    }

    return result;
}
