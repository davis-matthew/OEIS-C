/*
 * A167688  Number of ways of factoring n with all factors greater than 1
 *          (a(1)=1 by convention) minus number of nonprime divisors of n.
 * https://oeis.org/A167688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001055(n) – A033273(n) (number of unordered
 *            factorizations of n minus the number of nonprime divisors of n).
 *            For n = 1 the convention is a(1) = 0
 * Range:     u8 n <= 4319, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_and_prime_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_and_prime_counts.h"

/* Integer square root of a unsigned 64‑bit integer, using binary search
 * and avoiding overflow by using 128‑bit arithmetic for the square
 * comparison. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;          /* floor sqrt */
}

/* Count the unordered factorizations of n (A001055).  The function is
 * recursive and counts only factorizations with factors >1, treating
 * the factorisation consisting of n itself as one. */
static seq_t factor_count(uint64_t n, uint64_t min_factor)
{
    seq_t cnt = 1;                    /* the factorisation consisting of n itself */
    uint64_t limit = isqrt_u64(n);

    if (min_factor > limit)
        return cnt;

    for (uint64_t d = min_factor; d <= limit; ++d) {
        if (n % d == 0) {
            cnt += factor_count(n / d, d);
        }
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 by convention */
        return 0;

    uint64_t nn = (uint64_t)n;

    seq_t f = factor_count(nn, 2);          /* A001055(n) */
    seq_t tau, omega;
    divisor_and_prime_counts(nn, &tau, &omega);
    seq_t nonprime = tau - omega;           /* A033273(n) */
    return f - nonprime;
}
