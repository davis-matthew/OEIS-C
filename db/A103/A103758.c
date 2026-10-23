/*
 * A103758  Numbers n such that the string 100n is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A103758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th integer N such that 100·10^digits(N)+N is prime
 *            (i.e., the decimal string "100" followed by N is a prime)
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static const uint64_t pow10_buf[20] = {
    0ULL,                      /* unused, powers of 10 start at index 1 */
    10ULL,  100ULL,  1000ULL,  10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

seq_t a(idx_t n)
{
    idx_t target   = n;          /* target index (1‑based) */
    idx_t count    = 0;          /* how many hits found so far   */
    idx_t cand     = 1;          /* next candidate N             */
    idx_t digitlen = 1;          /* number of digits of cand     */
    uint64_t pow10 = pow10_buf[1];/* 10^digitlen                 */

    for (;;) {
        /* Construct the test prime: 100·10^digitlen + cand */
        uint64_t prime_candidate = 100ULL * pow10 + cand;

        if (is_prime_u64(prime_candidate)) {
            ++count;
            if (count == target)
                return (seq_t)cand;   /* found the n‑th term */
        }

        /* Advance to next candidate */
        ++cand;
        if (cand == pow10) {        /* reached next digit length */
            ++digitlen;
            pow10  = pow10_buf[digitlen];
        }
    }
}
