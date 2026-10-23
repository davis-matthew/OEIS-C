/*
 * A115788  a(n) = floor(n*Pi) mod 2.
 * https://oeis.org/A115788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*Pi) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(n*π) modulo 2 for any n ≥ 1.
   Uses an integer approximation of π:  π ≈ 3.141592653589793238.
   Scale factor = 10¹⁸ so that π_scaled = floor(π * 10¹⁸) fits in 64‑bit.
   The product n * π_scaled is held in a 128‑bit integer to avoid overflow.
   After division by the scale the result is the exact floor(n·π),
   and only its least‑significant bit is returned. */
#include <stdint.h>

static const unsigned long long SCALE      = 1000000000000000000ULL; /* 10¹⁸ */
static const unsigned long long PI_SCALED  = 3141592653589793238ULL;   /* floor(π·10¹⁸) */

seq_t a(idx_t n)
{
    /* multiply with 128‑bit precision */
    unsigned __int128 prod = (unsigned __int128)n * PI_SCALED;
    /* floor division by SCALE */
    unsigned __int128 val  = prod / SCALE;
    /* return parity */
    return (seq_t)(val & 1u);
}
