/*
 * A191156  a(n) = [6*n*Pi] - 2*[3*n*Pi], where [ ]=floor.
 * https://oeis.org/A191156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(6*n*Pi) - 2*floor(3*n*Pi)
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* High‑precision rational approximation of Pi:
 * Pi ≈ P / Q  with   P = 3141592653589793, Q = 1000000000000000
 * The absolute error is less than 1e-15, which guarantees that
 * floor(k*n*Pi) is correctly computed for all k ≤ 6 and n ≤ 10 000
 * using integer arithmetic only. */
static const unsigned long long PI_P = 3141592653589793ULL;
static const unsigned long long PI_Q = 1000000000000000ULL;

/* Compute floor(n * Pi) using integer arithmetic. */
static inline uint64_t floor_n_pi(idx_t n)
{
    /* use 128‑bit intermediate to avoid overflow */
    return (uint64_t)(((unsigned __int128)n * PI_P) / PI_Q);
}

seq_t a(idx_t n)
{
    uint64_t floor_3n = floor_n_pi(3 * n);
    uint64_t floor_6n = floor_n_pi(6 * n);
    /* the result is small enough to fit in seq_t by construction */
    return (seq_t)floor_6n - 2 * (seq_t)floor_3n;
}
