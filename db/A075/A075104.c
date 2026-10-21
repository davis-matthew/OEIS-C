/*
 * A075104  Greatest common divisor of n and integer part of log_2(n).
 * https://oeis.org/A075104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, ⌊log₂ n⌋)
 * Range:     u8 n <= 4048, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the built‑in GCD routine for seq_t values. */
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Helper: compute ⌊log₂ x⌋ for an unsigned 64‑bit integer.          */
/* We avoid any floating point or external header; a simple          */
/* shift loop is sufficient and fast for the 0 ≤ x ≤ ULLONG_MAX range. */
/* ------------------------------------------------------------------ */
static inline uint32_t floor_log2_u64(uint64_t x)
{
    uint32_t r = 0;
    /* Skip the highest 1‑bit: n > 0 always in this sequence. */
    while (x >>= 1)
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/* Main routine: compute a(n) for any n ≥ 1.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Cast the index to the sequence type. */
    seq_t n_seq = (seq_t)n;

    /* Compute the integer part of log₂(n). */
    uint32_t log2val = floor_log2_u64((uint64_t)n);
    seq_t log2_seq = (seq_t)log2val;

    /* Return the greatest common divisor of n and ⌊log₂ n⌋. */
    return gcd(n_seq, log2_seq);
}
