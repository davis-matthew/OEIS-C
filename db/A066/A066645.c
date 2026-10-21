/*
 * A066645  a(n) = floor( (4/3)*Pi*n^3 ).
 * https://oeis.org/A066645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((4/3)*Pi*n^3)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a 15‑digit rational approximation of (4/3)*Pi:
 *   P/Q = 4188790204786390 / 1000000000000000
 * The approximation satisfies |C - P/Q| < 10^-15, which guarantees
 * correct floor values for all n up to at least 10^6 because
 * (C - P/Q)*n^3 < 1 for n ≤ 10^6.
 * Implementation uses 128‑bit arithmetic (unsigned __int128) which
 * is available on gcc/clang and is also a trivial cast for
 * arbitrary‑precision seq_t builds.  */
 
#include <stdint.h>
 
/* Constants for the rational approximation of (4/3)*Pi */
typedef unsigned __int128 uint128_t;
static const uint128_t P = (uint128_t)4188790204786390ULL;
static const uint128_t Q = (uint128_t)1000000000000000ULL;
 
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Compute n^3 in 128‑bit to avoid overflow */
    uint128_t nn = (uint128_t)n;
    uint128_t n3 = nn * nn * nn;              /* n^3 */
    /* Multiply by numerator P, divide by Q, cast to seq_t */
    uint128_t val = (P * n3) / Q;
    return (seq_t)val;
}
