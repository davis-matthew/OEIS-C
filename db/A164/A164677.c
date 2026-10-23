/*
 * A164677  For a binary reflected Gray code, the (Hamming/Euclidean) distance
 *          between 2 subsequent points x and y is 1, say in coordinate k. If
 *          y has a 1 in coordinate k and x has a 0, than (x,y) is indicated
 *          by k, if it is the other way around, (x,y) is indicated by -k. The
 *          sequence has a fractal character...
 * https://oeis.org/A164677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p^e), where f(2^e) = e+1 f(p^e) =
 *            (-1)^(e*(p-1)/2) for odd primes p
 * Range:     i8 n <= 8192, i16 n <= 8192, i32 n <= 8192, i64 n <= 8192,
 *            i128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t   result = 1;
    int     e = 0;

    /* handle factor 2 */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        result *= (seq_t)(e + 1);
    }

    /* handle odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            uint64_t parity = ((uint64_t)e * ((p - 1) / 2)) & 1ULL;
            if (parity) {
                result = -result;
            }
        }
    }

    /* last prime factor > sqrt(original n) */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t parity = ((uint64_t)1 * ((p - 1) / 2)) & 1ULL;
        if (parity) {
            result = -result;
        }
    }

    return result;
}
