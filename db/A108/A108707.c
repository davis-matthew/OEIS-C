/*
 * A108707  Minimum side in Pythagorean triangles with hypotenuse of n.
 * https://oeis.org/A108707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum side of a Pythagorean triple with hypotenuse n.
 *            If no such triple exists, a(n) = 0. The algorithm precomputes
 *            the minimal leg for every odd integer h that can occur as the
 *            hypotenuse of a primitive triple. For general n, it considers
 *            all divisors d of n such that n/d is odd and use...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Precompute minimal leg of a primitive Pythagorean triple for each odd h ≤ 10000. */
static seq_t primitive_leg_table[10001] = {0};

/* Simple Euclidean GCD for small integers. */
static inline uint64_t small_gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the minimal leg for all odd h up to max_n. */
static void precompute_primitive_legs(idx_t max_n) {
    if (max_n > 10000) max_n = 10000;  /* Safety limit */
    for (idx_t h = 1; h <= max_n; ++h) {
        if ((h & 1ULL) == 0) continue; /* only odd primitive hypotenuses */

        seq_t best = 0;
        for (uint64_t m = 1; m * m <= h; ++m) {
            uint64_t rem = h - m * m;
            if (rem == 0) continue;            /* k must be > 0 */
            uint64_t k = (uint64_t)(sqrt((double)rem) + 0.5);
            if (k * k != rem) continue;        /* not a perfect square */
            if (m <= k) continue;              /* require m > k */
            if (small_gcd(m, k) != 1) continue;/* coprime */
            if (((m ^ k) & 1ULL) == 0) continue;/* opposite parity */

            seq_t a0 = (seq_t)(m * m - k * k);
            seq_t b0 = (seq_t)(2ULL * m * k);
            seq_t side = (a0 < b0) ? a0 : b0;
            if (best == 0 || side < best) best = side;
        }
        primitive_leg_table[h] = best;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety, though offset is 1 */

    /* Ensure precomputation up to 10000 only once */
    static int precomputed = 0;
    if (!precomputed) {
        precompute_primitive_legs(10000);
        precomputed = 1;
    }

    seq_t min_side = 0;  /* 0 indicates no triple found yet */

    /* Enumerate all divisors d of n */
    for (uint64_t d = 1; d * d <= (uint64_t)n; ++d) {
        if (n % d != 0) continue;

        /* First divisor d */
        uint64_t h1 = n / d;            /* primitive hypotenuse candidate */
        if ((h1 & 1ULL) == 1) {         /* must be odd */
            seq_t leg = primitive_leg_table[h1];
            if (leg != 0) {
                seq_t candidate = (seq_t)d * leg;
                if (min_side == 0 || candidate < min_side) {
                    min_side = candidate;
                }
            }
        }

        /* Second divisor n/d, if distinct */
        uint64_t d2 = n / d;
        if (d2 == d) continue;          /* same divisor, avoid double work */
        uint64_t h2 = n / d2;           /* which is d */
        if ((h2 & 1ULL) == 1) {
            seq_t leg = primitive_leg_table[h2];
            if (leg != 0) {
                seq_t candidate = (seq_t)d2 * leg;
                if (min_side == 0 || candidate < min_side) {
                    min_side = candidate;
                }
            }
        }
    }

    return min_side;
}
