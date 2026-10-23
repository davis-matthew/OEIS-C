/*
 * A183843  Number of (n+1)X8 binary arrays with each element of every 2X2
 *          subblock being the sum mod 2 of two others.
 * https://oeis.org/A183843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A183843 – number of (n+1)×8 binary arrays in which in every 2×2
    subblock each element equals the XOR (sum mod 2) of two other
    elements.  The sequence is computed by dynamic programming
    over successive rows: we pre‑compute which 8‑bit rows can follow
    each other, then iterate n steps (a matrix of (n+1) rows has
    n transitions).  The result is the total number of admissible
    (n+1)×8 arrays.  */

#include <stdint.h>
#include <stdbool.h>

/* Precomputed transition data */
static bool ready = false;
static bool allowed[16];
static uint16_t next_rows[256][256];
static uint8_t  next_count[256];

/* Determine if a 2×2 pattern satisfies the “XOR of two others”
   requirement for every element. */
static void compute_allowed_pattern(int pat)
{
    int b[4];
    b[0] = (pat >> 3) & 1;   /* top‑left      */
    b[1] = (pat >> 2) & 1;   /* top‑right     */
    b[2] = (pat >> 1) & 1;   /* bottom‑left   */
    b[3] =  pat        & 1;   /* bottom‑right  */

    bool ok = true;
    for (int e = 0; e < 4 && ok; ++e) {
        bool found = false;
        for (int f = 0; f < 4 && !found; ++f) {
            if (f == e) continue;
            for (int g = f + 1; g < 4 && !found; ++g) {
                if (g == e) continue;
                if (b[e] == (b[f] ^ b[g])) {
                    found = true;
                }
            }
        }
        if (!found) ok = false;
    }
    allowed[pat] = ok;
}

/* Build tables of admissible next rows. */
static void init(void)
{
    /* Which 4‑bit patterns are allowed? */
    for (int pat = 0; pat < 16; ++pat) {
        compute_allowed_pattern(pat);
    }

    /* For each 8‑bit row r1, list all compatibles r2. */
    for (int r1 = 0; r1 < 256; ++r1) {
        uint8_t cnt = 0;
        for (int r2 = 0; r2 < 256; ++r2) {
            bool ok = true;
            for (int j = 0; j < 7 && ok; ++j) {
                int a = (r1 >> j)     & 1;
                int b = (r1 >> (j+1)) & 1;
                int c = (r2 >> j)     & 1;
                int d = (r2 >> (j+1)) & 1;
                int pat = (a << 3) | (b << 2) | (c << 1) | d;
                if (!allowed[pat]) ok = false;
            }
            if (ok) {
                next_rows[r1][cnt++] = (uint16_t)r2;
            }
        }
        next_count[r1] = cnt;
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) init();

    static seq_t dp_cur[256];
    static seq_t dp_next[256];

    /* initial row: any 8‑bit row is allowed */
    for (int i = 0; i < 256; ++i) {
        dp_cur[i] = (seq_t)1;
    }

    /* iterate n transitions to build (n+1) rows */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i) {
            dp_next[i] = (seq_t)0;
        }
        for (int r1 = 0; r1 < 256; ++r1) {
            seq_t val = dp_cur[r1];
            if (val == (seq_t)0) continue;
            uint8_t cnt = next_count[r1];
            uint16_t *list = next_rows[r1];
            for (uint8_t t = 0; t < cnt; ++t) {
                int r2 = list[t];
                dp_next[r2] += val;
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < 256; ++i) {
            dp_cur[i] = dp_next[i];
        }
    }

    /* total number of admissible matrices */
    seq_t res = (seq_t)0;
    for (int i = 0; i < 256; ++i) {
        res += dp_cur[i];
    }
    return res;
}
