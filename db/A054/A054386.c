/*
 * A054386  Beatty sequence for Pi/(Pi-1); complement of A022844.
 * https://oeis.org/A054386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n * Π / (Π - 1)⌋, where Π ≈ 3.141592653589793238 This
 *            implementation uses a high‑precision integer approximation of Π
 *            scaled by 10^18. The intrinsic integer type __int128 guarantees
 *            that the intermediate product (n * Π_scaled) does not overflow
 *            for all indices that fit in the supported id...
 * Range:     u8 n <= 174, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used in the constants */

/* 10^18 scaling factor for Pi */
static const uint64_t PI10        = 3141592653589793238ULL; /* Π × 10^18 (truncated) */
static const uint64_t ONE10       = 1000000000000000000ULL; /* 1 × 10^18  */
static const uint64_t DELTA10     = PI10 - ONE10;          /* (Π – 1) × 10^18 */

/*------------------------------------------------------------*/
/*  a(n) = floor( n * Π / (Π – 1) )  (offset 1)              */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /*  n ≥ 1  (offset 1 is assumed by the caller)  */
    /*  Compute floor(n * PI10 / DELTA10) using 128‑bit arithmetic. */
    __int128_t prod = (__int128_t)n * PI10;
    seq_t result = (seq_t)(prod / DELTA10);
    return result;
}
