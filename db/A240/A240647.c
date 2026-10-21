/*
 * A240647  Number of nX6 0..1 arrays with no element equal to the same number
 *          of vertical neighbors as horizontal neighbors, with new values
 *          0..1 introduced in row major order.
 * https://oeis.org/A240647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the A240647 sequence: number of n×6 binary arrays where no
   element has the same number of equal vertical neighbours as equal
   horizontal neighbours.  The naive DP counts every array twice,
   hence the final result is divided by 2. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 6
#define PAT_COUNT (1U << WIDTH)
#define SENTINEL PAT_COUNT   /* index for missing row */

static int hor[PAT_COUNT][WIDTH];
static uint64_t mask_valid[SENTINEL + 1][PAT_COUNT];
static bool last_ok[SENTINEL + 1][PAT_COUNT];
static bool initialized = false;

static inline bool row_ok(uint8_t upPat, uint8_t curPat, uint8_t downPat,
                          bool upMissing, bool downMissing)
{
    for (int j = 0; j < WIDTH; ++j) {
        int hx = hor[curPat][j];            /* horizontal equal neighbours */
        int vert = 0;
        if (!upMissing && ((upPat >> j) & 1) == ((curPat >> j) & 1))
            ++vert;
        if (!downMissing && ((downPat >> j) & 1) == ((curPat >> j) & 1))
            ++vert;
        if (vert == hx)            /* forbidden if equal counts */
            return false;
    }
    return true;
}

static void precompute(void)
{
    /* horizontal equal neighbour counts */
    for (uint8_t p = 0; p < PAT_COUNT; ++p) {
        for (int j = 0; j < WIDTH; ++j) {
            int v = (p >> j) & 1;
            int cnt = 0;
            if (j > 0 && ((p >> (j - 1)) & 1) == v) ++cnt;
            if (j + 1 < WIDTH && ((p >> (j + 1)) & 1) == v) ++cnt;
            hor[p][j] = cnt;
        }
    }

    /* transition masks and validity of finishing a row */
    for (int up = 0; up <= SENTINEL; ++up) {
        bool upMissing = (up == SENTINEL);
        for (int cur = 0; cur < PAT_COUNT; ++cur) {
            uint64_t m = 0;
            for (int nxt = 0; nxt < PAT_COUNT; ++nxt) {
                if (row_ok((uint8_t)up, (uint8_t)cur, (uint8_t)nxt,
                           upMissing, false))
                    m |= (1ULL << nxt);
            }
            mask_valid[up][cur] = m;
            last_ok[up][cur] = row_ok((uint8_t)up, (uint8_t)cur, 0,
                                       upMissing, true);
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        precompute();

    static seq_t dp0[SENTINEL + 1][PAT_COUNT];
    static seq_t dp1[SENTINEL + 1][PAT_COUNT];
    seq_t (*cur)[PAT_COUNT] = dp0;
    seq_t (*next)[PAT_COUNT] = dp1;

    memset(cur, 0, sizeof(dp0));
    for (int p = 0; p < PAT_COUNT; ++p)
        cur[SENTINEL][p] = 1;

    for (idx_t row = 1; row < n; ++row) {
        memset(next, 0, sizeof(dp1));
        for (int up = 0; up <= SENTINEL; ++up) {
            for (int curIdx = 0; curIdx < PAT_COUNT; ++curIdx) {
                seq_t val = cur[up][curIdx];
                if (!val)
                    continue;
                uint64_t m = mask_valid[up][curIdx];
                while (m) {
                    uint8_t nxt = (uint8_t)__builtin_ctzll(m);
                    m &= m - 1;
                    next[curIdx][nxt] += val;
                }
            }
        }
        seq_t (*tmp)[PAT_COUNT] = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = 0;
    for (int up = 0; up <= SENTINEL; ++up) {
        for (int curIdx = 0; curIdx < PAT_COUNT; ++curIdx) {
            seq_t val = cur[up][curIdx];
            if (!val)
                continue;
            if (last_ok[up][curIdx])
                result += val;
        }
    }

    /* every array counted twice, divide by two */
    return result / 2;
}
