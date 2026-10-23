/*
 * A130855  2n appears 2n+1 times, 2n+1 appears 2n times.
 * https://oeis.org/A130855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A130855.
   For a given index n, find the block k where
        2k^2 - k ≤ n < 2(k+1)^2 - (k+1)
   Then:
        a(n) = 2k      if the offset within the block < 2k+1
        a(n) = 2k + 1  otherwise
   The block index k is ⌊(1 + ⌊√(1+8n)⌋)/4⌋, computed with integer arithmetic. */

#include <stdint.h>

/* Integer square root (floor) for 64‑bit unsigned integers.
   Uses a simple binary search over the bits. */
static inline uint64_t uint64_sqrt(uint64_t n) {
    uint64_t x = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t y = x | ((uint64_t)1 << i);
        if ((unsigned __int128)y * y <= n)
            x = y;
    }
    return x;
}

seq_t a(idx_t n) {
    /* k = floor((1 + floor(sqrt(1+8n))) / 4) */
    uint64_t t = 1 + 8 * n;               /* 1 + 8n fits in 64‑bits for n ≤ 10⁴ ; safe for n up to ~10¹⁴ */
    uint64_t s = uint64_sqrt(t);          /* floor(sqrt(1+8n)) */
    uint64_t k = (1 + s) >> 2;            /* integer division by 4 */

    uint64_t start = 2 * k * k - k;       /* cumulative length up to block (k–1) */
    uint64_t offset = n - start;          /* position inside block k */

    if (offset < 2 * k + 1)
        return (seq_t)(2 * k);
    else
        return (seq_t)(2 * k + 1);
}
