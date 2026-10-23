/*
 * A172276  a(n) = floor(n*(sqrt(13)-sqrt(5))).
 * https://oeis.org/A172276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(13)-sqrt(5)))
 * Range:     u8 n <= 186, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The constants below are chosen so that all intermediate
   values fit into 32 bits, while a 128‑bit product is used
   to avoid overflow for larger n. */

/* Scaling factor (3·10^9).  The choice guarantees
   SQRT13_SCALED - SQRT5_SCALED fits in 32 bits. */
static const uint64_t SCALE_64 = 3000000000ULL;

/* floor(sqrt(13)*SCALE_64) and floor(sqrt(5)*SCALE_64) */
static const uint64_t SQRT13_SCALED_64 = 10816653826ULL; /* sqrt(13) ≈ 3.6055512754639893 */
static const uint64_t SQRT5_SCALED_64  = 6708203932ULL;  /* sqrt(5) ≈ 2.23606797749979  */
static const uint64_t DIFF_64 = SQRT13_SCALED_64 - SQRT5_SCALED_64; /* 4118459894 */

seq_t a(idx_t n)
{
    /* 128‑bit intermediate to keep the product safe for all
       n that can be represented by the seq_t type. */
    __uint128_t prod = (__uint128_t)DIFF_64 * (__uint128_t)n;
    seq_t result = (seq_t)(prod / SCALE_64);
    return result;
}
