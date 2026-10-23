/*
 * A184102  n+floor(4*sqrt(n)); complement of A184103.
 * https://oeis.org/A184102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(4*sqrt(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The term floor(4*sqrt(n)) is computed as floor(sqrt(16*n)) using an 
 * integer binary‑search square‑root on an unsigned __int128 intermediate.
 * This keeps all arithmetic in integers, avoiding any floating‑point
 * operations. */

#include <stdint.h>

/* Binary search for floor(sqrt(x)) where x is a 128‑bit unsigned integer. */
static uint64_t sqrt_u128(unsigned __int128 x)
{
    /* The required square root is at most 2^34, because x ≤ 16*(2^64-1) < 2^68. */
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1U << 35);   /* safety upper bound */

    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;  /* upper mid to avoid endless loop */
        /* Use division to avoid overflow of mid*mid. */
        if ((unsigned __int128)mid <= x / mid) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

/* Compute a(n) = n + floor(4*sqrt(n)) for n >= 1. */
seq_t a(idx_t n)
{
    /* Compute 16 * n in 128‑bit to maintain precision. */
    unsigned __int128 target = (unsigned __int128)n * 16ULL;
    uint64_t s = sqrt_u128(target);        /* floor(sqrt(16*n)) == floor(4*sqrt(n)) */

    /* Add using 128‑bit intermediate to avoid overflow before casting. */
    unsigned __int128 result = (unsigned __int128)n + s;
    return (seq_t)result;
}
