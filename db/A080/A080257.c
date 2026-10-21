/*
 * A080257  Numbers having at least two distinct or a total of at least three
 *          prime factors.
 * https://oeis.org/A080257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number n-th integer that has ≥2 distinct prime factors or
 *            ≥3 prime factors in total. It is equivalent to the complement of
 *            the set of primes and prime squares. The function searches for
 *            the n‑th such integer by sieving primes up to a growing upper
 *            bound
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root (floor) using 128 bit arithmetic. */
static uint64_t isqrt_uint64(uint64_t n)
{
    if (n < 2)
        return n;
    uint64_t low = 0, high = (uint64_t)1 << 32;   /* sqrt(UINT64_MAX) < 2^32 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for zero, but return 0 safely */
        return 0;

    uint64_t target = n;
    uint64_t limit;
    /* minimum starting limit to cover the first few terms */
    limit = (target < 10) ? 20ULL : target * 2ULL;
    if (limit < 20ULL)
        limit = 20ULL;

    while (1) {
        /* Allocate and initialize sieve array */
        bool *isPrime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
        if (!isPrime)
            return 0;   /* memory allocation failed – unlikely */

        for (uint64_t i = 0; i <= limit; ++i)
            isPrime[i] = true;
        if (limit >= 0) {
            isPrime[0] = false;
            if (limit >= 1)
                isPrime[1] = false;
        }

        for (uint64_t p = 2; p * p <= limit; ++p) {
            if (isPrime[p]) {
                for (uint64_t j = p * p; j <= limit; j += p)
                    isPrime[j] = false;
            }
        }

        /* Count qualifying numbers until the target index is reached */
        uint64_t count = 0;
        uint64_t m = 0;
        for (m = 2; m <= limit; ++m) {
            if (isPrime[m])
                continue;                 /* skip primes */
            uint64_t r = isqrt_uint64(m);
            if (r * r == m && isPrime[r])
                continue;                 /* skip prime squares */
            ++count;
            if (count == target)
                break;
        }

        free(isPrime);

        if (count >= target)
            return (seq_t)m;               /* found the n-th term */

        /* Not enough numbers yet – increase the limit */
        if (limit == UINT64_MAX)
            break;                         /* safety guard */

        limit = (limit > UINT64_MAX / 2) ? UINT64_MAX : limit * 2;
    }
    return 0;   /* should never reach here for valid inputs */
}
