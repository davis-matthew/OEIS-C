/*
 * A120074  Row sums of triangle A120072 (numerator triangle for H atom
 *          spectrum).
 * https://oeis.org/A120074
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1}^{n-1} A120072(n,k) where A120072(n,k) is the
 *            numerator of the reduced fraction 1/k^2 - 1/n^2. The function
 *            computes this sum for all n >= 2
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for unsigned 64‑bit integers. */
static inline uint64_t gcd_ull(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sequence generator.  idx_t and seq_t are supplied by the build. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* Value undefined for n < 2 */

    seq_t sum = 0;
    uint64_t n_sq = (uint64_t)n * (uint64_t)n;

    for (idx_t k = 1; k < n; ++k) {
        uint64_t k_sq = (uint64_t)k * (uint64_t)k;

        /* numerator of the difference */
        uint64_t num = n_sq - k_sq;

        /* gcd(num, k^2 * n^2) is computed as gcd(num,k^2) * gcd(num/gcd(num,k^2), n^2) */
        uint64_t d1 = gcd_ull(num, k_sq);          /* gcd with k^2           */
        uint64_t num1 = num / d1;                  /* numerator after removing factors of k */
        uint64_t d2 = gcd_ull(num1, n_sq);         /* gcd with n^2           */
        uint64_t g   = d1 * d2;                    /* total gcd              */

        /* reduced numerator */
        seq_t red = (seq_t)(num / g);
        sum += red;
    }

    return sum;
}
