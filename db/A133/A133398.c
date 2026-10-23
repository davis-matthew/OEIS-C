/*
 * A133398  Numbers that are not Mersenne primes.
 * https://oeis.org/A133398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th number that is not a Mersenne prime.
   This is equivalent to finding the n‑th positive integer
   not in the set {2^p − 1 | 2^p − 1 is prime}.  It iterates
   forward until the count of numbers not excluded matches n. */
#include <stddef.h>
#include <stdint.h>

/* Known Mersenne primes that fit in 64 bits */
static const uint64_t mersenne_primes[] = {
    3ULL,
    7ULL,
    31ULL,
    127ULL,
    8191ULL,
    131071ULL,
    524287ULL,
    2147483647ULL,
    2305843009213693951ULL
};

static const size_t mersenne_primes_len =
    sizeof(mersenne_primes) / sizeof(mersenne_primes[0]);

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0, but safe */

    seq_t candidate = (seq_t)n;    /* first possible value */

    for (;; ++candidate) {
        /* Count how many Mersenne primes are <= candidate */
        size_t cnt = 0;
        for (size_t i = 0; i < mersenne_primes_len; ++i) {
            if (candidate >= (seq_t)mersenne_primes[i])
                ++cnt;
            else
                break;           /* array sorted ascending */
        }

        /* If candidate minus the count equals n, we found the n-th term */
        if (candidate - (seq_t)cnt == (seq_t)n)
            return candidate;
    }
}
