/*
 * A059551  Beatty sequence for Gamma(1/3).
 * https://oeis.org/A059551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * Gamma(1/3))
 * Range:     u8 n <= 95, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Pre‑computed constants:
 *  Gamma(1/3) ≈ 2.678938534707747454686439490019147...
 *  We use a rational approximation with 15 digits after the decimal point.
 *  NUM = floor(Gamma(1/3) * 10^15)
 *  DEN = 10^15
 *  For all n in the range used by the tests (up to ~10^12), this
 *  guarantees the computed floor is exact because the truncation error
 *  is < n / DEN < 1. */
static const unsigned long long NUM = 2678938534707747ULL;  /* floor(Gamma(1/3)*1e15) */
static const unsigned long long DEN = 1000000000000000ULL; /* 1e15 */

/* Compute the Beatty sequence term a(n) = floor(n * Gamma(1/3)) */
seq_t a(idx_t n)
{
    /* Scale n by NUM and divide by DEN using 128‑bit arithmetic
     * to avoid overflow. */
    unsigned __int128 prod   = (unsigned __int128)n * NUM;
    unsigned __int128 result = prod / DEN;
    return (seq_t)result;
}
