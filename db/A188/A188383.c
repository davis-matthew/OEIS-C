/*
 * A188383  Positions of 1 in the zero-one sequence [nr+3r]-[nr]-[3r], where
 *          r=1/sqrt(2).
 * https://oeis.org/A188383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer i such that floor((i+3)*r) –
 *            floor(i*r) = 2, with r = 1/√2
 * Range:     u8 n <= 31, u16 n <= 7950, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for 0 <= x < 2^64.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static uint64_t isqrt64(uint64_t x) {
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* 2^32 > sqrt(2^64) */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Helper: floor( i * r ) with r = 1/√2.
   Because i*r = floor( i*√2 / 2 ) = floor( floor(i√2)/2 ) and
   floor(i√2) = floor( sqrt(2*i*i) ) we can compute it exactly with
   integer arithmetic. */
static seq_t floor_r(uint64_t i) {
    unsigned __int128 val = (unsigned __int128)2 * i * i;  /* 2*i*i < 2^64 for i <= 10^5 */
    uint64_t a = isqrt64((uint64_t)val);                  /* floor( sqrt(2*i*i) ) = floor(i√2) */
    return (seq_t)(a >> 1);                                /* floor( floor(i√2)/2 ) = floor(i/√2) */
}

seq_t a(idx_t n) {
    /* The sequence is indexed from 1.  For n == 0 we return 0. */
    if (n == 0) return 0;

    idx_t i = 1;            /* candidate position of the next 1 */
    idx_t seen = 0;         /* how many 1's we have seen so far */

    while (1) {
        /* floor(i*r) and floor((i+3)*r) */
        seq_t fi  = floor_r((uint64_t)i);
        seq_t fip = floor_r((uint64_t)(i + 3));

        /* Test whether position i is a '1' in the sequence:
           floor((i+3)*r) – floor(i*r) – 2 == 1. */
        if (fip - fi == 3) {  /* fip - fi - 2 == 1  <=>  fip - fi == 3 */
            seen++;
            if (seen == n) {
                return (seq_t)i;
            }
        }
        i++;
    }
}
