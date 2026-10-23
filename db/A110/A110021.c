/*
 * A110021  Final term of the simple continued fraction for
 *          prime(n+1)/prime(n).
 * https://oeis.org/A110021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = last term of the simple continued fraction of
 *            prime(n+1)/prime(n)
 * Range:     u8 n <= 53, u16 n <= 7325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* out‑of‑range, caller should not ask for n < 1 */

    /* numerator = prime(n+1), denominator = prime(n) */
    uint64_t num = (uint64_t)get_prime(n + 1);
    uint64_t den = (uint64_t)get_prime(n);

    seq_t last = 0;
    while (den != 0) {
        uint64_t q = num / den;
        last = (seq_t)q;          /* current partial quotient */
        uint64_t r = num % den;   /* remainder */
        num = den;
        den = r;
    }
    return last;                   /* final partial quotient */
}
