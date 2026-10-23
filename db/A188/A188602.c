/*
 * A188602  Number of nX4 binary arrays without the pattern 1 1 0 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays that contain no pattern 1 1 0
 *            of length 3 in any horizontal, vertical, diagonal or
 *            antidiagonal direction. a(1)=12, a(2)=144, a(3)=847, …
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses a dynamic‑programming recurrence over the last two
    rows.  All intermediate values are kept in type seq_t, which may be
    8‑, 16‑, 32‑, 64‑ or 128‑bit unsigned, or an arbitrary‑precision
    integer type.  */
#include <stdbool.h>

static bool valid_row[16];
static uint8_t  next_rows[256][16];
static unsigned next_count[256];
static bool initialized = false;

/* Test whether a 4‑bit row satisfies the horizontal 110‑pattern rule */
static inline bool row_ok(uint8_t r)
{
    /* 110 at positions 0‑1‑2 */
    if ((r & 0x1) && (r & 0x2) && !(r & 0x4)) return false;
    /* 110 at positions 1‑2‑3 */
    if ((r & 0x2) && (r & 0x4) && !(r & 0x8)) return false;
    return true;
}

/* Pre‑compute valid rows and transition lists */
static void build_precomp(void)
{
    unsigned idx = 0;
    for (uint8_t r = 0; r < 16; ++r)
        valid_row[r] = row_ok(r);

    for (uint8_t a = 0; a < 16; ++a) {
        if (!valid_row[a]) continue;
        for (uint8_t b = 0; b < 16; ++b) {
            if (!valid_row[b]) continue;
            unsigned pair = (a << 4) | b;       /* 16*a + b  */
            unsigned cnt  = 0;
            for (uint8_t c = 0; c < 16; ++c) {
                if (!valid_row[c]) continue;

                /* vertical 110 pattern */
                bool bad = false;
                for (int j = 0; j < 4 && !bad; ++j) {
                    bool a1 = (a >> j) & 1;
                    bool b1 = (b >> j) & 1;
                    bool c0 = !((c >> j) & 1);
                    if (a1 && b1 && c0) bad = true;
                }

                /* down‑right diagonal 110 pattern */
                for (int j = 0; j < 2 && !bad; ++j) {
                    bool a1 = (a >> j) & 1;
                    bool b1 = (b >> (j + 1)) & 1;
                    bool c0 = !((c >> (j + 2)) & 1);
                    if (a1 && b1 && c0) bad = true;
                }

                /* down‑left (anti‑diagonal) 110 pattern */
                for (int j = 2; j < 4 && !bad; ++j) {
                    bool a1 = (a >> j) & 1;
                    bool b1 = (b >> (j - 1)) & 1;
                    bool c0 = !((c >> (j - 2)) & 1);
                    if (a1 && b1 && c0) bad = true;
                }

                if (!bad) {
                    next_rows[pair][cnt++] = c;
                    if (cnt == 16) break;   /* safety, though never happens */
                }
            }
            next_count[pair] = cnt;
        }
    }
    initialized = true;
}

/* Number of sequence elements of length 1 */
static inline unsigned count_valid_rows(void)
{
    unsigned c = 0;
    for (unsigned r = 0; r < 16; ++r) if (valid_row[r]) ++c;
    return c;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) build_precomp();

    if (n == 0) return 0;  /* not used, but harmless */

    /* base cases */
    if (n == 1) {
        return (seq_t)count_valid_rows();
    }
    if (n == 2) {
        unsigned v = count_valid_rows();
        return (seq_t)(v * v);
    }

    /* dp and dp_next store counts for each pair (prevrow, currrow) */
    static seq_t dp0[256];
    static seq_t dp1[256];
    for (unsigned i = 0; i < 256; ++i) dp0[i] = 0;

    unsigned v = count_valid_rows();
    /* initialise dp0 for length 2: all ordered pairs of valid rows */
    for (uint8_t a = 0; a < 16; ++a) {
        if (!valid_row[a]) continue;
        for (uint8_t b = 0; b < 16; ++b) {
            if (!valid_row[b]) continue;
            dp0[(a << 4) | b] = 1;
        }
    }

    for (idx_t row = 3; row <= n; ++row) {
        for (unsigned i = 0; i < 256; ++i) dp1[i] = 0;
        for (unsigned pair = 0; pair < 256; ++pair) {
            seq_t cnt = dp0[pair];
            if (cnt == 0) continue;
            uint8_t b = pair & 0xF;
            unsigned scnt = next_count[pair];
            for (unsigned t = 0; t < scnt; ++t) {
                uint8_t c = next_rows[pair][t];
                unsigned idx_next = ((b << 4) | c);
                dp1[idx_next] += cnt;
            }
        }
        /* swap dp0 and dp1 */
        for (unsigned i = 0; i < 256; ++i) dp0[i] = dp1[i];
    }

    /* sum over all states for length n */
    seq_t total = 0;
    for (unsigned i = 0; i < 256; ++i) total += dp0[i];
    return total;
}
