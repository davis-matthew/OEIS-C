/*
 * A231942  Number of 3Xn 0..3 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function counts the number of 3×n arrays with entries in 0..3 such that
   no element is less than a strict majority of its horizontal, diagonal and
   antidiagonal neighbours.  The count is obtained by dynamic programming over
   pairs of adjacent columns.
*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static void init_precomputation(void);
static inline uint8_t get_digit(uint8_t pat, unsigned r) { return (pat >> (2 * r)) & 3U; }
static inline bool first_ok(uint8_t a, uint8_t b);
static inline bool last_ok(uint8_t a, uint8_t b);
static inline bool middle_ok(uint8_t a, uint8_t b, uint8_t c);

static bool initialized = false;
static bool first_ok_tbl[64][64];
static bool last_ok_tbl[64][64];
static unsigned char nexts[64 * 64][64];
static unsigned char next_cnt[64 * 64] = {0};

static void init_precomputation(void)
{
    unsigned a, b, c;
    for (a = 0; a < 64; ++a)
        for (b = 0; b < 64; ++b) {
            first_ok_tbl[a][b] = first_ok(a, b);
            last_ok_tbl[a][b] = last_ok(a, b);
            for (c = 0; c < 64; ++c)
                if (middle_ok(a, b, c))
                    nexts[a * 64 + b][next_cnt[a * 64 + b]++] = (unsigned char)c;
        }
}

/* check constraint for leftmost column a using right neighbour b */
static inline bool first_ok(uint8_t a, uint8_t b)
{
    unsigned r;
    for (r = 0; r < 3; ++r) {
        uint8_t val = get_digit(a, r);
        unsigned better = 0;
        /* right neighbour */
        if (get_digit(b, r) > val) ++better;
        /* up-right */
        if (r > 0 && get_digit(b, r - 1) > val) ++better;
        /* down-right */
        if (r < 2 && get_digit(b, r + 1) > val) ++better;
        unsigned k = 1 + (r > 0) + (r < 2);
        if (better > k / 2) return false;
    }
    return true;
}

/* check constraint for rightmost column b using left neighbour a */
static inline bool last_ok(uint8_t a, uint8_t b)
{
    unsigned r;
    for (r = 0; r < 3; ++r) {
        uint8_t val = get_digit(b, r);
        unsigned better = 0;
        /* left neighbour */
        if (get_digit(a, r) > val) ++better;
        /* up-left */
        if (r > 0 && get_digit(a, r - 1) > val) ++better;
        /* down-left */
        if (r < 2 && get_digit(a, r + 1) > val) ++better;
        unsigned k = 1 + (r > 0) + (r < 2);
        if (better > k / 2) return false;
    }
    return true;
}

/* check constraint for middle column b given left a and right c */
static inline bool middle_ok(uint8_t a, uint8_t b, uint8_t c)
{
    unsigned r;
    for (r = 0; r < 3; ++r) {
        uint8_t val = get_digit(b, r);
        unsigned better = 0;
        /* left */
        if (get_digit(a, r) > val) ++better;
        /* right */
        if (get_digit(c, r) > val) ++better;
        /* up-left */
        if (r > 0 && get_digit(a, r - 1) > val) ++better;
        /* down-left */
        if (r < 2 && get_digit(a, r + 1) > val) ++better;
        /* up-right */
        if (r > 0 && get_digit(c, r - 1) > val) ++better;
        /* down-right */
        if (r < 2 && get_digit(c, r + 1) > val) ++better;
        unsigned k = 2 + 2 * (r > 0) + 2 * (r < 2);
        if (better > k / 2) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_precomputation();
        initialized = true;
    }

    if (n == 1) /* all 4^3 arrays are admissible */
        return (seq_t)64;

    /* DP arrays over states (previous column, current column) */
    static seq_t dp_cur[4096];
    static seq_t dp_next[4096];
    memset(dp_cur, 0, sizeof(dp_cur));

    /* initialise with first two columns */
    unsigned a, b;
    for (a = 0; a < 64; ++a)
        for (b = 0; b < 64; ++b)
            if (first_ok_tbl[a][b])
                dp_cur[a * 64 + b] = (seq_t)1;

    /* iterate columns 3 .. n */
    idx_t col;
    for (col = 2; col < n; ++col) {
        memset(dp_next, 0, sizeof(dp_next));
        for (a = 0; a < 64; ++a) {
            for (b = 0; b < 64; ++b) {
                unsigned idx = a * 64 + b;
                seq_t val = dp_cur[idx];
                if (val == 0) continue;
                unsigned nc = next_cnt[idx];
                unsigned char *list = nexts[idx];
                unsigned j;
                for (j = 0; j < nc; ++j) {
                    unsigned c = list[j];
                    dp_next[b * 64 + c] += val;
                }
            }
        }
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* sum over last column constraints */
    seq_t ans = 0;
    for (a = 0; a < 64; ++a)
        for (b = 0; b < 64; ++b)
            if (last_ok_tbl[a][b])
                ans += dp_cur[a * 64 + b];

    return ans;
}
