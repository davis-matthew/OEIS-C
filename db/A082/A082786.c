/*
 * A082786  Triangle, read by rows, of exponents of primes in canonical prime
 *          factorization of n: T(n,k) = greatest number such that
 *          prime(k)^T(n,k) divides n, 1 <= k <= n.
 * https://oeis.org/A082786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = exponent of prime(k) in the prime factorization of row r,
 *            where r is the smallest integer with r(r+1)/2 >= n and k = n −
 *            r(r−1)/2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Find the row r: smallest integer with r(r+1)/2 >= n. */
    idx_t low = 1;
    idx_t high = n;               /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column k within the row (1‑based). */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev);  /* 1 <= k <= r */

    /* Exponent of prime(k) in the factorization of r. */
    uint32_t p = get_prime(k);    /* kth prime: 2,3,5,7,... */
    seq_t exp = 0;
    idx_t temp = r;
    while (temp % p == 0) {
        ++exp;
        temp /= p;
    }
    return exp;
}
