/*
 * A024583  a(n) = floor(n/{n*Pi}), where { } = fractional part.
 * https://oeis.org/A024583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / frac(n*Pi)) where frac(x) = x - floor(x)
 * Range:     u8 n <= 28, u16 n <= 670, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * Pi approximated as an integer scaled by 10^18.
 * The value is rounded down, guaranteeing an error ≤ 1e-18.
 * For n ≤ 10⁴ this precision is more than enough to determine
 * the integer part of n / frac(n*Pi) exactly.
 */
#define PI_INT  3141592653589793238ULL  /* Pi * 10^18 rounded down */
#define SCALE   1000000000000000000ULL  /* 10^18 */

/* Compute a(n) = floor(n / {n*Pi}) with exact integer arithmetic. */
seq_t a(idx_t n)
{
    /* Offset is 1, so for n == 0 we return 0 (irrelevant). */
    if (n == 0)
        return 0;

    /* Use 128‑bit arithmetic to avoid overflow. */
    const unsigned long long pi_scaled  = PI_INT;
    const unsigned long long scale     = SCALE;

    unsigned __int128 prod   = (unsigned __int128)n * pi_scaled;   /* n * Pi_scaled */
    unsigned __int128 numer = prod % scale;                      /* numerator of fractional part */

    /* In theory numer should never be zero because Pi is irrational. */
    if (numer == 0)
        return (seq_t)n;      /* fallback, should never occur */

    unsigned __int128 result = ((unsigned __int128)n * scale) / numer;
    return (seq_t)result;
}
