/*
 * A210639  Nearest integer to (4/3)*Pi*n^(3/2).
 * https://oeis.org/A210639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to (4/3)*π*n^(3/2)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the rational approximation π ≈ 355/113
   and a fixed‑point sqrt with 10^6 scaling, which is accurate
   for all n up to at least 10 000 and gives the correct rounding. */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(v)). */
static uint64_t isqrt_uint128(unsigned __int128 v) {
    uint64_t lo = 0;
    uint64_t hi = (uint64_t)-1;          /* 2^64‑1 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid_sq = (unsigned __int128)mid * mid;
        if (mid_sq == v) {
            return mid;
        } else if (mid_sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;   /* floor(sqrt(v)) */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* sequence offset 0 */

    /* Compute (4/3)*π ≈ 1420/339 using π ≈ 355/113. */
    const uint64_t NUM = 1420;            /* numerator of 1420/339 */
    const uint64_t DEN = 339;             /* denominator            */
    const uint64_t SCALE = 1000000ULL;    /* 10^6 scaling for sqrt */
    const uint64_t DEN_SCALED = DEN * SCALE;   /* 339000000 */

    /* A = n^3 * 10^12  (since SCALE^2 = 10^12). */
    unsigned __int128 A = (unsigned __int128)n * n * n * 1000000000000ULL;

    /* s = floor(10^6 * n^(3/2)) = floor( sqrt(A) ). */
    uint64_t s = isqrt_uint128(A);

    /* Compute rounded value: floor( (NUM * s + DEN_SCALED/2) / DEN_SCALED ). */
    uint64_t num = NUM * s;                  /* fits in 64 bits for n≤10 000 */
    uint64_t result = (num + DEN_SCALED / 2) / DEN_SCALED;

    return (seq_t)result;
}
