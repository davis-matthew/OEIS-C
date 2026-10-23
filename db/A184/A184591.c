/*
 * A184591  a(n) = floor(n*(Pi-1)-1); complement of A184592.
 * https://oeis.org/A184591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(Pi-1)-1)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a fixed-point approximation of Pi-1 with scaling 10^16.
   The multiplication is performed in 128-bit to avoid overflow for
   64‑bit builds.  The result fits in seq_t when it fits in the target type. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Scaling factor 10^16; Pi-1 ≈ 2.141592653589793238..., so
       floor((Pi-1) * 10^16) = 21415926535897932. */
    const uint64_t SCALING = 10000000000000000ULL;
    const uint64_t NUM     = 21415926535897932ULL;     /* floor((Pi-1)*SCALING) */

    if (n == 0)   /* offset is 1; behaviour below offset is irrelevant */
        return 0;

    /* Use unsigned 128‑bit to hold the product n * NUM safely. */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    uint64_t floor_val = (uint64_t)(prod / SCALING);

    /* a(n) = floor(n*(Pi-1)) - 1 */
    return (seq_t)(floor_val - 1);
}
