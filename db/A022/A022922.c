/*
 * A022922  Number of integers m such that 5^n < 2^m < 5^(n+1).
 * https://oeis.org/A022922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*log₂(5)) – floor(n*log₂(5))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* implemented with integer arithmetic to avoid floating point               */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* scale factor: 10¹² is large enough that the truncation error    */
    /* (≈1/10¹²) times n is far below 0.5 for all n ≤ 10 000.            */
    const uint64_t SCALE = 1000000000000ULL;
    /* M = floor(log₂(5) * SCALE).  log₂(5) ≈ 2.321928094887362347920039 */
    const uint64_t M = 2321928094887ULL;   /* floor(2.321928094887362.. × 10¹²) */

    /* Calculate floor values using 128‑bit arithmetic to avoid overflow */
    __int128 left   = (__int128)(n + 1) * M;
    __int128 right  = (__int128)n * M;

    seq_t a_val = (seq_t)((left / SCALE) - (right / SCALE));
    return a_val;
}
