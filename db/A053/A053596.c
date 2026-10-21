/*
 * A053596  Open 3-dimensional ball numbers (version 4): a(n) is the number of
 *          integer points (i,j,k) contained in an open ball of diameter n,
 *          centered at (1/2, 1/2, 1/2).
 * https://oeis.org/A053596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integer triples (i,j,k) with (i-0.5)^2 +
 *            (j-0.5)^2 + (k-0.5)^2 < (n/2)^2
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This counts the unimodular lattice points in an open ball of
    diameter n centred at (½,½,½).  The implementation
    iterates over odd coordinates and uses an integer
    square‑root routine to count admissible z‑values. */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for 32‑bit unsigned values.
   Returns floor(sqrt(n)). */
static uint32_t isqrt32(uint32_t n) {
    uint32_t x = n, y;
    if (x == 0)
        return 0;
    y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    /* For small n the result is zero. */
    if (n < 2)
        return (seq_t)0;

    /* Work in 32‑bit arithmetic (n ≤ 10 000 in expected tests). */
    uint32_t N      = (uint32_t)n;
    uint32_t N2     = N * N;        /* n² ≤ 1e8 */
    seq_t   total   = 0;

    for (int32_t x = 1; x < (int32_t)N; x += 2) {
        uint32_t X2 = (uint32_t)x * (uint32_t)x;
        for (int32_t y = 1; y < (int32_t)N; y += 2) {
            uint32_t Y2 = (uint32_t)y * (uint32_t)y;
            int32_t rem = (int32_t)N2 - (int32_t)X2 - (int32_t)Y2;
            if (rem <= 1)
                continue;           /* No admissible z at all. */
            uint32_t t = isqrt32((uint32_t)(rem - 1));
            /* Largest odd ≤ t. */
            uint32_t odd_max = (t & 1) ? t : (t > 0 ? t - 1 : 0);
            if (odd_max < 1)
                continue;
            uint32_t odd_cnt = (odd_max + 1) >> 1;  /* Count of odd |z| ≤ odd_max. */
            uint32_t z_cnt   = odd_cnt << 1;        /* Positive + negative. */

            /* 4 sign choices for x and y, 2 for z counted already. */
            total += (seq_t)(4) * (seq_t)z_cnt;
        }
    }

    return total;
}
