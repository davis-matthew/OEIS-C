/*
 * A072514  Sum of n mod k for k in {1...n} with gcd(k,n) > 1.
 * https://oeis.org/A072514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n, gcd(k,n)>1} (n mod k)
 * Range:     u8 n <= 53, u16 n <= 713, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses an O(n) loop and an inline Euclid algorithm. */

#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k) {
        if (gcd64(n, k) > 1) {
            sum += (seq_t)(n % k);
        }
    }
    return sum;
}
