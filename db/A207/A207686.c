/*
 * A207686  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 43, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207686: Number of 6×n 0..1 arrays avoiding 
 * horizontal 0 0 0 and 1 0 1, and vertical 0 0 1 and 1 1 1.
 * Computation via transfer‑matrix DP. 
 * The state is a pair of consecutive columns (prev, curr).
 * Allowed columns (vertical constraints) are 18 in total.
 */

#include <stdint.h>
#include <stdbool.h>

static const int MAX_COL = 6;          /* number of rows in a column */
static const int COLMASK = 0x3F;       /* mask for 6 bits */

/* Check vertical patterns in a single column */
static bool vertical_ok(unsigned char bits) {
    for (int r = 0; r <= MAX_COL - 3; ++r) {
        int a = (bits >> r) & 1;
        int b = (bits >> (r + 1)) & 1;
        int c = (bits >> (r + 2)) & 1;
        if (a == 0 && b == 0 && c == 1) return false; /* 0 0 1 */
        if (a == 1 && b == 1 && c == 1) return false; /* 1 1 1 */
    }
    return true;
}

/* Check horizontal patterns across three consecutive columns */
static bool horizontal_ok(unsigned char a, unsigned char b, unsigned char c) {
    for (int r = 0; r < MAX_COL; ++r) {
        int x = (a >> r) & 1;
        int y = (b >> r) & 1;
        int z = (c >> r) & 1;
        if (x == 0 && y == 0 && z == 0) return false; /* 0 0 0 */
        if (x == 1 && y == 0 && z == 1) return false; /* 1 0 1 */
    }
    return true;
}

seq_t a(idx_t n) {
    /* static data initialized once */
    static bool initialized = false;
    static unsigned char allowed[64];          /* column patterns */
    static unsigned char idx_of[64];           /* map bits -> index */
    static int M = 0;                          /* number of allowed columns */
    static bool trans[64][64][64];             /* transition table */
    static seq_t dp_prev[64 * 64];
    static seq_t dp_next[64 * 64];

    if (!initialized) {
        /* build allowed columns */
        for (int bits = 0; bits < 64; ++bits) {
            idx_of[bits] = (unsigned char)-1;
        }
        for (int bits = 0; bits < 64; ++bits) {
            if (vertical_ok((unsigned char)bits)) {
                allowed[M] = (unsigned char)bits;
                idx_of[bits] = (unsigned char)M;
                ++M;
            }
        }

        /* build transition table */
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                for (int k = 0; k < M; ++k) {
                    trans[i][j][k] = horizontal_ok(
                        allowed[i], allowed[j], allowed[k]);
                }
            }
        }
        /* zero DP arrays */
        for (int i = 0; i < M * M; ++i) {
            dp_prev[i] = 0;
            dp_next[i] = 0;
        }

        initialized = true;
    }

    if (n == 0) return 0; /* offset 1, behaviour below not required */

    /* n == 1 and n == 2 are trivial */
    if (n == 1) return (seq_t)M;
    if (n == 2) return (seq_t)M * (seq_t)M;

    /* initialise DP for two columns */
    for (int i = 0; i < M * M; ++i) {
        dp_prev[i] = 1; /* all pairs of allowed columns are possible */
    }

    /* iterate from column 3 to n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero next layer */
        for (int i = 0; i < M * M; ++i) {
            dp_next[i] = 0;
        }

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t val = dp_prev[i * M + j];
                if (val == 0) continue;
                for (int k = 0; k < M; ++k) {
                    if (!trans[i][j][k]) continue;
                    dp_next[j * M + k] += val;
                }
            }
        }
        /* copy next to prev for next iteration */
        for (int i = 0; i < M * M; ++i) {
            dp_prev[i] = dp_next[i];
        }
    }

    /* sum all DP states to get the total number for n columns */
    seq_t total = 0;
    for (int i = 0; i < M * M; ++i) {
        total += dp_prev[i];
    }
    return total;
}
