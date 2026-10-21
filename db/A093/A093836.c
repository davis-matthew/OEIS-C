/*
 * A093836  Numerator of A000328(n)/n^2, where A000328(n) is the number of
 *          lattice points (x,y) with x^2 + y^2 <= n^2.
 * https://oeis.org/A093836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 145, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093836:
   a(n) = numerator of A000328(n) / n^2,
   where A000328(n) is the number of integer lattice points (x, y)
   with x^2 + y^2 ≤ n^2.
   The implementation uses an O(n) summation over |x| ≤ n and
   exact integer arithmetic.  It works for n up to about 100 000
   (well within the required 10 000 limit) and for any build
   where seq_t can hold the result. */
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/floor_sqrt_u64.h"

/* Euclidean gcd for 64‑bit integers */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification */
    uint64_t n64 = (uint64_t)n;
    uint64_t n_sq = n64 * n64;              /* n^2 */
    uint64_t sum = 0;

    /* sum_{x=1}^{n} floor( sqrt(n^2 – x^2) ) */
    for (uint64_t x = 1; x <= n64; ++x) {
        uint64_t diff = n_sq - x * x;
        sum += floor_sqrt_u64(diff);
    }

    /* total lattice points: 4*sum + 4n + 1 */
    uint64_t N = 4 * sum + 4 * n64 + 1ULL;

    /* reduce the fraction N / n^2 and return the numerator */
    uint64_t g = gcd_u64(N, n_sq);
    uint64_t numer = N / g;

    return (seq_t)numer;
}
