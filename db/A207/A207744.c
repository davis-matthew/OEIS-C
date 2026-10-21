/*
 * A207744  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207744: Number of 6×n binary arrays avoiding
 * horizontal 0 0 0 and 0 1 0, vertical 0 0 1 and 1 0 0.
 * The implementation uses a transfer‑matrix over pairs of columns.
 * The code is free of look‑up tables and works for any n≥1.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Validates that a 6‑bit column contains no vertical forbidden pattern. */
static int vertical_ok(uint8_t col)
{
    int i;
    for (i = 2; i < 6; ++i) {
        int a = (col >> (i - 2)) & 1;
        int b = (col >> (i - 1)) & 1;
        int c = (col >> i) & 1;
        if (a == 0 && b == 0 && c == 1) return 0;  /* 0 0 1 */
        if (a == 1 && b == 0 && c == 0) return 0;  /* 1 0 0 */
    }
    return 1;
}

/* Main function required by the assignment */
seq_t a(idx_t n)
{
    /* Static state – initialized once */
    static uint8_t V;                /* number of valid columns */
    static uint8_t *col_bits;        /* column bit patterns, length V */
    static uint8_t *needMask;        /* mask of rows that must be 1 for the next column, size V*V */
    static seq_t *cur, *next;        /* DP buffers, size V*V */
    static int init = 0;

    if (!init) {
        /* Enumerate all valid columns */
        uint8_t tmp[64];
        int cnt = 0;
        for (int i = 0; i < 64; ++i)
            if (vertical_ok((uint8_t)i)) tmp[cnt++] = (uint8_t)i;
        V = (uint8_t)cnt;

        /* Allocate arrays */
        col_bits = (uint8_t *)malloc(V * sizeof(uint8_t));
        memcpy(col_bits, tmp, V * sizeof(uint8_t));

        size_t states = (size_t)V * (size_t)V;
        needMask = (uint8_t *)malloc(states * sizeof(uint8_t));

        /* Pre‑compute per‑pair requirement masks */
        size_t idx = 0;
        for (int i1 = 0; i1 < V; ++i1) {
            uint8_t col1 = col_bits[i1];
            for (int i2 = 0; i2 < V; ++i2) {
                uint8_t col2 = col_bits[i2];
                uint8_t mask = 0;
                for (int r = 0; r < 6; ++r) {
                    int a = (col1 >> r) & 1;
                    int b = (col2 >> r) & 1;
                    if (a == 0 && b == 0)          mask |= 1 << r;  /* need new bit 1 */
                    if (a == 0 && b == 1)          mask |= 1 << r;  /* need new bit 1 */
                }
                needMask[idx++] = mask;
            }
        }

        /* Allocate DP tables */
        cur  = (seq_t *)calloc(states, sizeof(seq_t));
        next = (seq_t *)calloc(states, sizeof(seq_t));

        init = 1;
    }

    /* Handle trivial cases directly */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)V;
    if (n == 2) return (seq_t)V * (seq_t)V;

    /* Initialize DP for columns 1 and 2 (all pairs admissible) */
    size_t states = (size_t)V * (size_t)V;
    size_t i;
    for (i = 0; i < states; ++i) cur[i] = 1;

    /* Add columns 3 … n */
    for (size_t step = 2; step < (size_t)n; ++step) {
        /* zero next layer */
        memset(next, 0, states * sizeof(seq_t));

        for (i = 0; i < states; ++i) {
            seq_t ways = cur[i];
            if (!ways) continue;

            uint8_t need = needMask[i];
            size_t prev1 = i % V;        /* second column of the state */
            size_t base  = prev1 * (size_t)V;

            for (size_t k = 0; k < V; ++k) {
                uint8_t bits = col_bits[k];
                /* new column must satisfy all required bits */
                if ((bits & need) != need) continue;
                size_t newid = base + k;
                next[newid] += ways;
            }
        }

        /* swap buffers */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Sum over all final states */
    seq_t total = 0;
    for (i = 0; i < states; ++i) total += cur[i];
    return total;
}
