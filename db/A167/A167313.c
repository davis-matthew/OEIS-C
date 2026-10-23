/*
 * A167313  Totally multiplicative sequence with a(p) = 3*(p-3) for prime p.
 * https://oeis.org/A167313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Π_{p^e∥n} (3·(p−3))^e (and a(3k)=0 for k≥1)
 * Range:     i8 n <= 31, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n % 3 == 0) return (seq_t)0;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Handle factor 2 separately for speed */
    int e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        e++;
    }
    if (e > 0) {
        seq_t base = (seq_t)(3 * ((seq_t)2 - 3));  /* -3 */
        seq_t term = (seq_t)1;
        for (int i = 0; i < e; ++i) term *= base;
        result *= term;
    }

    /* Enumerate odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            /* p==3 would have already zeroed the result */
            seq_t base = (seq_t)(3 * ((seq_t)p - 3));
            seq_t term = (seq_t)1;
            for (int i = 0; i < e; ++i) term *= base;
            result *= term;
        }
    }

    if (m > 1) {           /* remaining prime factor */
        uint64_t p = m;
        /* p==3 impossible here because n%3!=0 */
        seq_t base = (seq_t)(3 * ((seq_t)p - 3));
        result *= base;   /* exponent is 1 */
    }

    return result;
}
