/*
 * A162146  a(n) = the smallest positive integer divisible by exactly n
 *          distinct primes, where each of these primes has the same number of
 *          digits when written in binary.
 * https://oeis.org/A162146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = product of the first n primes that lie in the smallest
 *            interval [2^(k-1), 2^k – 1] containing at least n primes. The
 *            interval defines the binary length (k bits) of all primes used.
 *            The result is square‑free and minimal by construction
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Helper: linear search for the first prime >= value.  This is fast enough
   because the total number of primes below the used limit (~1 048 576) is
   only ~78 000. */
static size_t first_ge(const uint64_t *beg, const uint64_t *end, uint64_t value)
{
    size_t i = 0;
    while (beg[i] < value) ++i;
    return i;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;               /* a(0) = 1 */

    /* Static prime cache – generated once on first call. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    /* Build a list of all primes below LIMIT if not yet built. */
    const idx_t LIMIT = ((idx_t)1 << 20);   /* 1 048 576, enough for n up to ~40 000 */
    if (!primes) {
        build_primes_upto(LIMIT, &primes, &primes_cnt);
    }

    /* Find the minimal binary length k such that the interval
       [2^(k-1), 2^k-1] contains at least n primes. */
    idx_t k = 1;
    distance_loop:
    {
        uint64_t low  = (k == 1) ? 1ULL : (uint64_t)1 << (k - 1);
        uint64_t high = ((uint64_t)1 << k) - 1;
        size_t   a = first_ge(primes, primes + primes_cnt, low);
        size_t   b = a;
        while (b < primes_cnt && primes[b] <= high) ++b;
        size_t cnt = b - a;
        if (cnt < n) {
            ++k;
            goto distance_loop;
        }
        /* Compute the product of the first n primes in this interval. */
        seq_t prod = (seq_t)1;
        for (idx_t i = 0; i < n; ++i) {
            prod *= (seq_t)primes[a + i];
        }
        return prod;
    }
}
