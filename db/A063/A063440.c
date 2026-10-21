/*
 * A063440  Number of divisors of n-th triangular number.
 * https://oeis.org/A063440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(A000217(n)) = τ(n(n+1)/2) * Since n and n+1 are
 *            coprime, τ(n(n+1)/2) = * τ(n/2)·τ(n+1) if n is even, *
 *            τ(n)·τ((n+1)/2) otherwise
 * Range:     u8 n <= 2078, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for uint64_t using binary search, safe against overflow. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) {
            return mid;
        } else if (sq < n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;   /* high < low and high*high <= n */
}

/* Divisor count of a uint64_t value.  Result is promoted to seq_t. */
static seq_t divisor_cnt_u64(uint64_t x)
{
    seq_t tau = 1;
    /* factor 2 */
    if ((x & 1) == 0) {
        uint64_t e = 0;
        while ((x & 1) == 0) {
            ++e;
            x >>= 1;
        }
        tau *= (seq_t)(e + 1);
    }

    /* odd primes */
    uint64_t limit = isqrt_u64(x);
    for (uint64_t p = 3; p <= limit; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                ++e;
                x /= p;
            }
            tau *= (seq_t)(e + 1);
            if (x == 1) break;
            limit = isqrt_u64(x); /* keep loop bound tight */
        }
    }

    if (x > 1) {
        /* remaining prime factor */
        tau *= (seq_t)2;
    }
    return tau;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, safety only */

    uint64_t n_val = (uint64_t)n;
    uint64_t part1, part2;

    if ((n_val & 1) == 0) {               /* n even: use n/2 and n+1 */
        part1 = n_val / 2;
        part2 = n_val + 1;
    } else {                              /* n odd: use n and (n+1)/2 */
        part1 = n_val;
        part2 = (n_val + 1) / 2;
    }

    seq_t tau1 = divisor_cnt_u64(part1);
    seq_t tau2 = divisor_cnt_u64(part2);

    return tau1 * tau2;
}
