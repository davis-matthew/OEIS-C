/*
 * A075437  Triangle read by rows giving successive iterations of the Rule 110
 *          elementary cellular automaton starting with a single black (1)
 *          cell where row n is of length 2n+1.
 * https://oeis.org/A075437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth bit in the triangle of Rule 110 elementary
 *            cellular automaton, where rows are read by rows starting with a
 *            single 1 at row 0
 * Range:     u8 n <= 9820, u16 n <= 9820, u32 n <= 9820, u64 n <= 9820,
 *            u128 n <= 9820, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle rows have lengths 1,3,5,…; a(n) is the (n‑th) element of the flattened
   triangle.  The algorithm simulates the automaton up to the required row. */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root (floor) for the unsigned type idx_t.  */
static inline idx_t isqrt_idx(idx_t n) {
    idx_t lo = 0;
    /* hi is set such that hi*hi > n for all 64‑bit n.  */
    idx_t hi = ((idx_t)1 << 32);
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* Rule 110 table: index = left<<2 | center<<1 | right.  */
static const uint8_t rule110[8] = {0,1,1,1,0,1,1,0};

seq_t a(idx_t n) {
    /* Determine the row number r and the position k within that row.  */
    idx_t r = isqrt_idx(n);                 /* floor(sqrt(n))          */
    __int128 rr = (__int128)r * (__int128)r;
    idx_t k = n - (idx_t)rr;                /* 0‑based index in row r */

    /* Row 0 is trivial.  */
    if (r == 0)
        return (seq_t)1;

    /* Allocate buffers big enough for the largest row reached.  */
    size_t max_len = 2 * r + 1;             /* length of row r */
    uint8_t *buf1 = (uint8_t *)malloc(max_len);
    uint8_t *buf2 = (uint8_t *)malloc(max_len);
    if (!buf1 || !buf2) {
        /* Allocation failed; return 0 as a safe fallback.  */
        if (buf1) free(buf1);
        if (buf2) free(buf2);
        return (seq_t)0;
    }

    /* Initialise row 0.  */
    size_t len = 1;
    buf1[0] = 1;

    /* Simulate successive rows until we reach row r.  */
    for (idx_t step = 0; step < r; ++step) {
        size_t next_len = len + 2;
        for (size_t j = 0; j < next_len; ++j) {
            uint8_t left   = (j >= 2 && j - 2 < len) ? buf1[j - 2] : 0;
            uint8_t center = (j >= 1 && j - 1 < len) ? buf1[j - 1] : 0;
            uint8_t right  = (j < len) ? buf1[j] : 0;
            buf2[j] = rule110[(left << 2) | (center << 1) | right];
        }
        /* Swap buffers for next iteration.  */
        uint8_t *tmp  = buf1;
        buf1 = buf2;
        buf2 = tmp;
        len = next_len;
    }

    seq_t result = (seq_t)buf1[k];

    free(buf1);
    free(buf2);
    return result;
}
