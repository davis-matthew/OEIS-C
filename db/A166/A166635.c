/*
 * A166635  Totally multiplicative sequence with a(p) = 5*(p-1) for prime p.
 * https://oeis.org/A166635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (5·(p−1))^e (multiplicative with a(p)=5·(p−1))
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the sequence value for index n (1-based). */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;        /* a(1)=1 */

    idx_t m = n;
    seq_t result = (seq_t)1;
    idx_t exp, i;

    /* Handle factor 2 separately. */
    if ((m & 1ULL) == 0) {
        exp = 0;
        while ((m & 1ULL) == 0) {
            ++exp;
            m >>= 1;
        }
        seq_t base = (seq_t)5 * (seq_t)(2 - 1); /* 5·1 = 5 */
        seq_t pow = 1;
        for (i = 0; i < exp; ++i) pow *= base;
        result *= pow;
    }

    /* Process odd prime factors. */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            seq_t base = (seq_t)5 * (seq_t)(p - 1);
            seq_t pow = 1;
            for (i = 0; i < exp; ++i) pow *= base;
            result *= pow;
        }
    }

    /* If a prime factor larger than sqrt(original n) remains. */
    if (m > 1) {
        seq_t base = (seq_t)5 * (seq_t)(m - 1);
        result *= base;
    }

    return result;
}
