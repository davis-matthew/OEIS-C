/*
 * A134730  Successive digits of Sophie Germain primes A005384(n).
 * https://oeis.org/A134730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th digit of the infinite string formed by writing
 *            all Sophie Germain primes in decimal without separators
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static inline uint64_t pow10_u64(idx_t e)
{
    uint64_t res = 1;
    while (e--) res *= 10ULL;
    return res;
}

seq_t a(idx_t n)
{
    /* iterate SG primes until the n-th digit is found */
    idx_t total_digits = 0;     /* digits already accounted for */
    uint64_t p = 1;             /* start searching from 2 */

    for (;;)
    {
        /* find next Sophie Germain prime */
        do
        {
            ++p;
        } while (!(is_prime(p) && is_prime(2ULL * p + 1ULL)));

        /* number of decimal digits of p */
        uint64_t tmp = p;
        idx_t len = 0;
        do
        {
            ++len;
            tmp /= 10ULL;
        } while (tmp != 0ULL);

        /* check if the desired digit lies in this prime */
        if (total_digits + len >= n)
        {
            idx_t offset = n - total_digits - 1;      /* 0‑based inside p */
            idx_t exp    = len - 1 - offset;          /* power of ten */
            uint64_t div  = pow10_u64(exp);
            uint64_t digit = (p / div) % 10ULL;
            return (seq_t)digit;
        }

        /* accumulate digits and continue */
        total_digits += len;
    }
}
