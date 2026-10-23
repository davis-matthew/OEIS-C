/*
 * A022933  Number of e^m between 2^n and 2^(n+1).
 * https://oeis.org/A022933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1) * ln 2) - floor(n * ln 2)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using integer arithmetic only (no floating point). */

#include <stdint.h>

static const uint64_t DEN = 1000000000000ULL;  /* 10^12 */
static const uint64_t LN2_NUM = 693147180559ULL; /* floor(ln 2 * 10^12) */

seq_t a(idx_t n)
{
    /* Compute floor(n * ln 2) and floor((n+1) * ln 2) using fixed‑point */
    unsigned __int128 prod_n  = (unsigned __int128)n         * LN2_NUM;
    unsigned __int128 prod_np = (unsigned __int128)(n + 1) * LN2_NUM;

    seq_t f_n  = (seq_t)(prod_n  / DEN);  /* floor(n * ln 2)   */
    seq_t f_np = (seq_t)(prod_np / DEN);  /* floor((n+1) * ln 2) */

    return f_np - f_n;  /* 0 or 1 for the given range of n */
}
