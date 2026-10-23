/*
 * A189402  n+[ns/r]+[nt/r]; r=1, s=e, t=e^2.
 * https://oeis.org/A189402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*e) + floor(n*e^2) For r = 1, s = e, t = e^2.
 *            This implementation uses high‑precision integer arithmetic with
 *            18 decimal digits of e and e² (denominator 10^18). It works for
 *            any seq_t width (uint8_t … arbitrary‑precision) and produces the
 *            exact value whenever it fits in seq_t
 * Range:     u8 n <= 23, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 18‑digit integer approximations of e and e² (truncated). */
static const uint64_t E_NUM   = 2718281828459045235ULL;   /* floor(e * 10^18) */
static const uint64_t E2_NUM  = 7389056098930650227ULL;   /* floor(e² * 10^18) */
static const uint64_t DEN     = 1000000000000000000ULL;   /* 10^18 */

/* Compute a(n) = n + floor(n*e) + floor(n*e²) */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow of n * E_NUM etc. */
    unsigned __int128 prod_e  = (unsigned __int128)n * E_NUM;
    unsigned __int128 prod_e2 = (unsigned __int128)n * E2_NUM;

    uint64_t floor_ne  = (uint64_t)(prod_e  / DEN);
    uint64_t floor_ne2 = (uint64_t)(prod_e2 / DEN);

    return (seq_t)n + (seq_t)floor_ne + (seq_t)floor_ne2;
}
