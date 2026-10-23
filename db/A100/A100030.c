/*
 * A100030  Bisection of A008472.
 * https://oeis.org/A100030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of distinct primes dividing 2*n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* If n==0 (outside the offset) the result is undefined,
       but we return 0 for safety. */
    if (n == 0) return 0;

    /* 2 is always a prime factor of 2*n. */
    seq_t sum = 2;

    /* Remove the factor 2 from n to avoid adding it twice. */
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }

    /* Factorise the remaining part of n. */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            sum += (seq_t)p;          /* add this distinct prime */
            while (m % p == 0) {      /* remove all copies of p */
                m /= p;
            }
        }
    }

    /* If anything remains, it is a prime factor larger than sqrt(n). */
    if (m > 1) {
        sum += (seq_t)m;
    }

    return sum;
}
