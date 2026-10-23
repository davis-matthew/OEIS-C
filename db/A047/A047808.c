/*
 * A047808  a(n) counts different values of i^2 + j^2 <= n^2 or number of
 *          distances from the origin to all integer points inside a circle of
 *          radius n.
 * https://oeis.org/A047808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) counts the distinct integer values of i^2 + j^2 with i,j ≥
 *            0 that do not exceed n^2. Equivalent to the number of integers ≤
 *            n^2 expressible as a sum of two squares
 * Range:     u8 n <= 27, u16 n <= 535, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Compute floor(sqrt(x)) for 64‑bit unsigned integer */
static inline idx_t isqrt_u64(idx_t x) {
    return (idx_t)sqrt((double)x);
}

/* seq_t is an unsigned integer type supplied by the build; no
   arithmetic beyond what the type can hold is performed.  */
seq_t a(idx_t n)
{
    /* maximum representable sum */
    idx_t limit = n * n;
    /* bitset size in bytes: one bit per integer 0 … limit */
    size_t bitset_bytes = (limit >> 3) + 1;
    uint8_t *bits = (uint8_t*)malloc(bitset_bytes);
    if (!bits) return 0;           /* allocation failure – unlikely */
    memset(bits, 0, bitset_bytes);

    seq_t count = 0;

    for (idx_t i = 0; i <= n; ++i) {
        idx_t i2 = i * i;
        idx_t maxJ = isqrt_u64(limit - i2);
        for (idx_t j = 0; j <= maxJ; ++j) {
            idx_t s = i2 + j * j;
            uint8_t *p   = bits + (s >> 3);
            uint8_t  mask = 1u << (s & 7);
            if ((*p & mask) == 0) {
                *p |= mask;
                ++count;
            }
        }
    }

    free(bits);
    return count;
}
