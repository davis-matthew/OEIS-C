/*
 * A123931  a(n) = H(n)*n!/(floor(n/2))! (mod (n+1)), where H(n) = sum{k=1 to
 *          n} 1/k, the n-th harmonic number.
 * https://oeis.org/A123931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = H(n)*n!/(floor(n/2))! mod (n+1)
 * Range:     u8 n <= 307, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sequence by explicit combinatorial summation. */

#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
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
    if (n == 0) return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t m  = nn / 2;           /* floor(n/2)            */
    uint64_t mod = nn + 1;          /* (n+1) for the modulus */
    uint64_t sum = 0;

    for (uint64_t k = 1; k <= nn; ++k) {
        uint64_t rem = k;           /* remaining factor to cancel */
        uint64_t prod = 1;          /* product of reduced t's modulo mod */

        for (uint64_t t = m + 1; t <= nn; ++t) {
            uint64_t g = gcd_u64(t, rem);
            if (g > 1) {
                rem /= g;           /* cancel common factor */
            }
            uint64_t t_red = t / g; /* reduced multiplicand */
            prod = (prod * t_red) % mod;
        }
        /* rem should be 1 here; the term is integer */
        sum += prod;
        sum %= mod;
    }

    return (seq_t)sum;
}
