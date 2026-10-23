/*
 * A045698  Number of ways n can be written as the sum of two squares of
 *          primes.
 * https://oeis.org/A045698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways n can be written as the sum of two squares of primes.
   a(n) = |{ (p, q) : p and q are primes, p ≤ q, p² + q² = n }| */

#include <stdint.h>
#include <stdlib.h>

/* integer square root (floor) for 64‑bit unsigned values */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* The smallest sum of two prime squares is 2² + 2² = 8.  For n < 8
       we can short‑circuit, but the following code works for all n. */
    idx_t limit = isqrt(n);
    if (limit < 2) return 0;

    /* Simple sieve of Eratosthenes up to 'limit'.  */
    /* Note: a static allocation of too many bytes is problematic when
       'limit' is very large, but for the required ranges (n up to ~10⁴)
       the memory is trivial. */
    unsigned char *isprime = (unsigned char *)malloc((size_t)(limit + 1));
    if (isprime == NULL) return 0;  /* allocation failure → 0 */

    for (idx_t i = 0; i <= limit; ++i) isprime[i] = 1;
    if (limit >= 0) isprime[0] = 0;
    if (limit >= 1) isprime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                isprime[j] = 0;
            }
        }
    }

    seq_t count = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!isprime[p]) continue;
        idx_t p2 = p * p;
        if (p2 > n) break;          /* any larger 'p' would exceed n */

        for (idx_t q = p; q <= limit; ++q) {
            if (!isprime[q]) continue;
            idx_t q2 = q * q;
            idx_t sum = p2 + q2;
            if (sum == n) {
                ++count;
            } else if (sum > n) {
                /* For fixed 'p', increasing 'q' only increases the sum. */
                break;
            }
        }
    }

    free(isprime);
    return count;
}
