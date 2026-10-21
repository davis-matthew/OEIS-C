/*
 * A210436  Number of digits in 6^n.
 * https://oeis.org/A210436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor( n * log10(6) ) + 1 Number of decimal digits in
 *            6^n. Implements integer arithmetic only using a fixed‑point
 *            approximation of log10(6) with 18 fractional digits
 * Range:     u8 n <= 327, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Approximate log10(6) ≈ 0.778151250383643632 */
    /* Represented as NUM/DEN with NUM and DEN fitting in uint64_t. */
    static const uint64_t NUM = 778151250383643632ULL;     /* 0.778151250383643632 * 10^18 */
    static const uint64_t DEN = 1000000000000000000ULL;    /* 10^18 */

    /* Compute floor( n * NUM / DEN ) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    uint64_t floor_val = (uint64_t)(prod / DEN);

    return (seq_t)(floor_val + 1);
}
