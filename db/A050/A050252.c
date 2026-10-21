/*
 * A050252  Number of digits in the prime factorization of n (counting terms
 *          of the form p^1 as p).
 * https://oeis.org/A050252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of decimal digits in the prime factorisation of n,
 *            * counting a factor p¹ as the single digit for p (no exponent
 *            shown)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_count_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless in this context */
    if (n == 1) return 1;   /* 1 is represented by a single digit */

    seq_t sum = 0;
    idx_t m = n;            /* work on a copy of n */

    /* Factor 2 separately. */
    if ((m & 1ULL) == 0) {
        sum += 1;           /* the prime 2 itself is one digit */
        idx_t e = 0;
        while ((m & 1ULL) == 0) {   /* while divisible by 2 */
            m >>= 1;                /* n /= 2 */
            e++;
        }
        if (e > 1) sum += digit_count_uint64(e);  /* exponent digits */
    }

    /* Factor odd primes. */
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            sum += digit_count_uint64(i);  /* prime digits */
            idx_t e = 0;
            while (m % i == 0) {
                m /= i;
                e++;
            }
            if (e > 1) sum += digit_count_uint64(e);  /* exponent digits */
        }
    }

    /* If anything remains, it is a prime > sqrt(original n). */
    if (m > 1)
        sum += digit_count_uint64(m);

    return sum;
}
