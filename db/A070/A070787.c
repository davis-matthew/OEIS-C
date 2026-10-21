/*
 * A070787  Number of triangles with sides whose squares are integers and with
 *          positive integer area and longest side of length sqrt(n).
 * https://oeis.org/A070787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (a,b) with 1 <= a <= b <= n *
 *            such that the triangle with side lengths sqrt(a), sqrt(b),
 *            sqrt(n) * has positive integer area. The area condition is
 *            equivalent to * * 16*A^2 = 2(ab + bn + na) - (a^2 + b^2 + n^2) *
 *            * and must be a perfect square; moreover that squar...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   is_square_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the minimal headers required for math and integer types. */
#include <math.h>
#include <stdbool.h>
#include "../utils/is_square_uint64.h"

/* Main sequence accessor. */
seq_t a(idx_t n)
{
    /* If n is 0 or negative (under the offset) return 0. */
    if (n == 0) return (seq_t)0;

    uint64_t un = (uint64_t)n;           /* local copy as 64‑bit unsigned */
    uint64_t count = 0;                  /* accumulate result */

    for (uint64_t a = 1; a <= un; ++a) {
        for (uint64_t b = a; b <= un; ++b) {
            /* Triangle inequality: sqrt(a)+sqrt(b) > sqrt(n). */
            if (un > a + b) {
                uint64_t diff = un - a - b;
                uint64_t lhs  = diff * diff;
                uint64_t rhs  = 4 * a * b;
                if (lhs >= rhs) continue;   /* fails inequality */
            }

            /* Compute M = 16*A^2 = 2(ab + bn + na) - (a^2 + b^2 + n^2). */
            __uint128_t ab = (__uint128_t)a * (__uint128_t)b;
            __uint128_t an = (__uint128_t)a * (__uint128_t)un;
            __uint128_t bn = (__uint128_t)b * (__uint128_t)un;
            __uint128_t a2 = (__uint128_t)a * (__uint128_t)a;
            __uint128_t b2 = (__uint128_t)b * (__uint128_t)b;
            __uint128_t n2 = (__uint128_t)un * (__uint128_t)un;

            __uint128_t M128 = 2 * (ab + an + bn) - (a2 + b2 + n2);
            if (M128 == 0) continue;      /* area 0 (degenerate) */

            /* M must be divisible by 16. */
            if ((M128 & 15) != 0) continue;

            uint64_t M = (uint64_t)(M128 >> 4);  /* M / 16   (A^2) */

            /* Check that M is a perfect square. */
            uint64_t root;
            if (is_square_uint64(M, &root)) {
                ++count;                /* valid triangle found */
            }
        }
    }

    return (seq_t)count;
}
