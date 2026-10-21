/*
 * A202998  Previous integer to m^2/n where m is the next odd prime after n (n
 *          excluded).
 * https://oeis.org/A202998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = previousinteger( nextoddprime(n)^2 / n ) where
 *            nextoddprime(n) is the next odd prime strictly greater than n
 * Range:     u8 n <= 12, u16 n <= 298, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether x is a prime number, assuming x fits in uint64_t.
   Only simple trial division is needed for n up to about 10⁶. */
static bool is_prime_uint64(uint64_t x)
{
    if (x <= 1) return false;
    if (x <= 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Find the next odd prime strictly greater than n. */
    uint64_t p = n + 1;
    if (p <= 2) p = 3;
    if ((p & 1) == 0) ++p;          /* make it odd */
    while (!is_prime_uint64(p)) {
        p += 2;                     /* skip even numbers */
    }

    seq_t sq = (seq_t)p * p;        /* p^2 */
    seq_t denom = (seq_t)n;
    seq_t q = sq / denom;           /* integer division */
    seq_t r = sq % denom;           /* remainder */

    if (r == 0) --q;                /* previous integer when exact */
    return q;
}
