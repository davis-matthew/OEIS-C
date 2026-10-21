/*
 * A054842  If n = a + 10 * b + 100 * c + 1000 * d + ... then a(n) = (2^a) *
 *          (3^b) * (5^c) * (7^d) * ...
 * https://oeis.org/A054842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy, look
 * Formula:   a(n) = Π_{i≥0} prime(i+1)^{digit_i(n)} (digit_0 = least
 *            significant decimal digit)
 * Range:     u8 n <= 7, u16 n <= 58, u32 n <= 498, u64 n <= 8898,
 *            u128 n <= 9999, big any n
 * Helpers:   _pow_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  - first prime is 2, second 3, third 5, and so on.  */
/*  - For n = 0 the function returns 1.  */
/*  - The implementation does a simple digit‑by‑digit scan of n in base 10.  */

#include <stdint.h>
#include "../utils/_pow_digit.h"

static const unsigned int _primes20[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;               /* a(0) = 1 */

    seq_t result = 1;
    unsigned int pos = 0;       /* position of the digit (0 = units) */

    while (n != 0) {
        unsigned int digit = n % 10;   /* least significant decimal digit */
        if (pos >= 20)                /* safety check – should never happen for uint64_t */
            return result;            /* or handle error in other builds */

        seq_t prime = (seq_t)_primes20[pos];
        if (digit != 0)
            result *= _pow_digit(prime, digit);

        n /= 10;
        ++pos;
    }

    return result;
}
