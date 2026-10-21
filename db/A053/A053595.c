/*
 * A053595  Open 3-dimensional ball numbers (version 3): a(n) is the number of
 *          integer points (i,j,k) contained in an open ball of diameter n,
 *          centered at (1/2,1/2,0).
 * https://oeis.org/A053595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integer points (i,j,k) in the open ball of
 *            diameter n centered at (1/2,1/2,0). Equivalent to counting
 *            integer triples (x,y,z) with x,y odd, z even such that x² + y² +
 *            z² < n². The implementation uses simple integer arithmetic and
 *            nested loops that run in O(n²)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(v)) for 0 <= v <= 2^64-1.  */
static inline uint64_t isqrt_u64(uint64_t v) {
    uint64_t r = 0;
    for (int b = 31; b >= 0; --b) {
        uint64_t bit = 1ULL << b;
        uint64_t cand = r + bit;
        if (cand * cand <= v) {
            r = cand;
        }
    }
    return r;
}

/* Count odd‑odd lattice points (x,y) with x² + y² < r², where r² > 0.  */
static inline uint64_t count_pairs(uint64_t r2) {
    uint64_t maxX = isqrt_u64(r2 - 1);          /* |x| < sqrt(r2)  */
    uint64_t sum = 0;
    /* iterate over positive odd x only, double later for ±x */
    for (uint64_t x = 1; x <= maxX; x += 2) {
        uint64_t rem = r2 - x * x;               /* r2 - x²  > 0 */
        uint64_t maxY = isqrt_u64(rem - 1);      /* |y| < sqrt(rem) */
        uint64_t odd_y = 2 * ((maxY + 1) / 2);   /* count of odd y in [-maxY, maxY] */
        sum += odd_y;
    }
    return sum * 2;                               /* include ±x */
}

/* Main function for sequence A053595.  */
seq_t a(idx_t n) {
    /* For n = 0 or 1, the ball is too small to contain any integer points. */
    if (n < 2) {
        return 0;
    }

    /* Use 128-bit where necessary to avoid overflow, but all values here fit
       comfortably into 64 bits for n up to 10 000. */
    uint64_t nn = (uint64_t)n * (uint64_t)n;          /* n²          */
    uint64_t kmax = (n - 1) / 2;                      /* floor((n-1)/2) */

    uint64_t total = 0;
    for (uint64_t k = 0; k <= kmax; ++k) {
        uint64_t z2 = 4ULL * k * k;                    /* (2k)²          */
        if (z2 >= nn) {                               /* r2 ≤ 0         */
            continue;
        }
        uint64_t r2 = nn - z2;                        /* remaining radius² */

        uint64_t pairs = count_pairs(r2);              /* points for this k (±x handled) */

        if (k == 0) {                                 /* only one sign for z */
            total += pairs;
        } else {                                       /* include ±k */
            total += 2 * pairs;
        }
    }

    return (seq_t)total;
}
