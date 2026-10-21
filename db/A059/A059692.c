/*
 * A059692  Table of carryless products i * j, i>=0, j>=0, read by
 *          antidiagonals.
 * https://oeis.org/A059692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 573, u16 n <= 9869, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   carryless_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059692 – Table of carryless products read by antidiagonals
 *
 *  a(n) = (i ⊗ j) where (i,j) is the unique pair with i+j equal to
 *  the diagonal number s, and i is the offset of the index along that
 *  diagonal (see comments below).  ⊗ denotes carry‑less multiplication
 *  in base‑10: each decimal digit of the product is (di * dj) mod 10
 *  and there is no carry propagation.
 *
 *  The diagonal containing index n is determined by the largest s such
 *  that s(s+1)/2 ≤ n.  This is s = ⌊(√(8n+1)−1)/2⌋.  The offset of
 *  n inside that diagonal is offset = n − s(s+1)/2, and the corresponding
 *  pair is (i = offset, j = s−offset).
 *
 *  The function computes the carry‑less product on the fly, so it works
 *  for any seq_t type.  No external dependencies are required apart
 *  from the standard headers.
 */

#include <stdint.h>
#include "../utils/carryless_product.h"

/* Integer square root of a 64‑bit unsigned integer: floor(sqrt(x)) */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x >> 1 | 1;
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)   /* mid*mid > x avoids overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find diagonal index s: largest s with s(s+1)/2 ≤ n.  */
    uint64_t disc = 8 * n + 1;                     /* 8n+1 fits in 64‑bit for n ≤ 10⁴ */
    uint64_t root = isqrt_u64(disc);               /* floor(sqrt(8n+1))   */
    idx_t    s    = (root - 1) / 2;                /* s = floor((root-1)/2)  */

    /* Compute offset of n within diagonal s.  */
    idx_t start  = (s * (s + 1)) / 2;              /* index of first term on diag s */
    idx_t offset = n - start;                      /* 0 ≤ offset ≤ s */

    idx_t i = offset;
    idx_t j = s - offset;

    return carryless_product(i, j);
}
