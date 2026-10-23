/*
 * A033272  Number of odd nonprimes dividing n.
 * https://oeis.org/A033272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# odd divisors of n) - (# distinct odd prime divisors of
 *            n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any n ≥ 1 and uses only integer arithmetic. */
#include <stdint.h>

/* Integer square root for 64‑bit numbers using binary search and __int128. */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t low = 0, high = 4294967296ULL;  /* 2^32 is > sqrt(2^64−1) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* outside the defined offset */

    /* Strip factors of 2 – they do not affect odd divisors. */
    idx_t odd = n;
    while ((odd & 1ULL) == 0) odd >>= 1;

    seq_t odd_divisors = 1;        /* product of (exponent+1) for odd primes */
    idx_t distinct = 0;            /* distinct odd prime factors */

    idx_t temp = odd;
    idx_t limit = isqrt_u64(temp);
    for (idx_t p = 3; p <= limit; p += 2) {
        if (temp % p == 0) {
            ++distinct;
            idx_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            odd_divisors *= (seq_t)(exp + 1);
            limit = isqrt_u64(temp);  /* limit shrinks as temp gets smaller */
        }
    }

    /* Remaining prime factor (if any). */
    if (temp > 1) {
        ++distinct;
        odd_divisors *= 2;          /* exponent is 1 → (1+1) */
    }

    return odd_divisors - (seq_t)distinct;
}
