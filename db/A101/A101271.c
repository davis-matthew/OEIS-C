/*
 * A101271  Number of partitions of n into 3 distinct and relatively prime
 *          parts.
 * https://oeis.org/A101271
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into 3 distinct relatively
 *            prime parts (computed by brute‑force search over a<b<c)
 * Range:     u8 n <= 58, u16 n <= 892, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Count of partitions of n into 3 distinct and relatively prime parts */
seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* values below the offset are undefined */

    seq_t count = 0;

    for (idx_t a_val = 1; a_val <= n - 3; ++a_val) {
        /* b must satisfy a_val < b < c and a_val + b + c = n.
           Hence b <= (n - a_val - 1) / 2 ensures c > b. */
        idx_t max_b = (n - a_val - 1) / 2;
        for (idx_t b = a_val + 1; b <= max_b; ++b) {
            idx_t c = n - a_val - b;          /* c > b by construction of max_b */

            uint64_t g = gcd_uint64(a_val, b);
            g = gcd_uint64(g, c);
            if (g == 1)
                ++count;
        }
    }
    return count;
}
