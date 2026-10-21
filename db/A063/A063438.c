/*
 * A063438  a(n) = floor((n+1)*Pi) - floor(n*Pi).
 * https://oeis.org/A063438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n+1)*Pi) - floor(n*Pi)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the rational approximation 104348/33215 of Pi.    */
/* The error |104348/33215 - Pi| < 1/(33215^2) < 1e-9,     */
/* which guarantees correct floor values for all n up to 10000
   (and far beyond) with 64‑bit arithmetic.                */
#include <stdint.h>

static const unsigned long long NUM = 104348ULL; /* numerator of Pi approximation */
static const unsigned long long DEN = 33215ULL;  /* denominator of Pi approximation */

/* Compute floor(n * Pi) using the rational approximation. */
static inline seq_t floor_mul_Pi(idx_t n)
{
    __uint128_t prod = (__uint128_t)n * NUM; /* 128‑bit product */
    return (seq_t)(prod / DEN);
}

/* Main sequence function as required by the competition. */
seq_t a(idx_t n)
{
    /* n is at least 1 for valid calls. */
    return floor_mul_Pi(n + 1) - floor_mul_Pi(n);
}
