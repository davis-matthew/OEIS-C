/*
 * A227065  The number of partitions of 2n into exactly two parts such that
 *          the smaller and larger parts are not both prime.
 * https://oeis.org/A227065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - number of Goldbach partitions of 2n, i.e., count of
 *            unordered prime pairs (p, 2n-p) with p <= n
 * Range:     u8 n <= 270, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    // Total unordered partitions of 2n into two parts.
    idx_t total = n;

    // Count Goldbach partitions where both parts are prime.
    idx_t gold = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime(p))
            continue;
        idx_t q = 2 * n - p;
        if (is_prime(q))
            ++gold;
    }

    return (seq_t)(total - gold);
}
