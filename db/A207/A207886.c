/*
 * A207886  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Transfer‑matrix DP for 4×n binary arrays avoiding
    horizontal patterns 000,101 and vertical patterns 011,110.
    The state is the last two columns (each 4 bits).
    Complexity: O(n·256·9) ≈ 2·10⁷ operations for n≈10⁴.
*/

#include <stdbool.h>

static bool init_done = false;
static unsigned char allowed[16];          /* 1 if column allowed, 0 otherwise */
static int allowed_list[16];              /* list of allowed column values */
static int allowed_cnt;                   /* number of allowed columns */

static unsigned char trans[16][16][16];   /* 1 if prev,curr,new allowed */

/* (Re)initialize tables on first call */
static void init_tables(void)
{
    if (init_done) return;

    /* Determine allowed columns (vertical constraint) */
    allowed_cnt = 0;
    for (int col = 0; col < 16; ++col) {
        bool ok = true;
        for (int r = 0; r < 2; ++r) {          /* 2 triples in a 4‑row column */
            int trip = (col >> r) & 0x7;        /* bits (r,r+1,r+2) */
            if (trip == 0x3 || trip == 0x6) {   /* 011 or 110 forbidden */
                ok = false;
                break;
            }
        }
        if (ok) {
            allowed[col] = 1;
            allowed_list[allowed_cnt++] = col;
        } else {
            allowed[col] = 0;
        }
    }

    /* Precompute horizontal transitions */
    for (int p = 0; p < 16; ++p) {
        for (int c = 0; c < 16; ++c) {
            for (int n = 0; n < 16; ++n) {
                unsigned char ok = 1;
                if (!allowed[p] || !allowed[c] || !allowed[n]) ok = 0;
                else {
                    for (int r = 0; r < 4; ++r) {
                        int trip = ((p >> r) & 1) << 2 |
                                   ((c >> r) & 1) << 1 |
                                   ((n >> r) & 1);
                        if (trip == 0 || trip == 5) { /* 000 or 101 forbidden */
                            ok = 0;
                            break;
                        }
                    }
                }
                trans[p][c][n] = ok;
            }
        }
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;               /* offset starts at 1, undefined otherwise */

    if (n == 1)
        return (seq_t)allowed_cnt;

    if (n == 2) {
        seq_t c = (seq_t)allowed_cnt;
        return c * c;
    }

    /* DP arrays: counts for sequences ending with columns (prev,cur) */
    seq_t ways[16][16] = {0};
    seq_t ways_next[16][16] = {0};

    /* Initialize for length = 2: all allowed pairs count as 1 */
    for (int p = 0; p < allowed_cnt; ++p)
        for (int c = 0; c < allowed_cnt; ++c)
            ways[allowed_list[p]][allowed_list[c]] = 1;

    /* Extend columns up to n */
    for (idx_t i = 3; i <= n; ++i) {
        /* zero next array */
        for (int r = 0; r < 16; ++r)
            for (int c = 0; c < 16; ++c)
                ways_next[r][c] = 0;

        for (int p = 0; p < 16; ++p) {
            for (int c = 0; c < 16; ++c) {
                seq_t val = ways[p][c];
                if (val == 0) continue;
                for (int ncol = 0; ncol < 16; ++ncol) {
                    if (trans[p][c][ncol]) {
                        ways_next[c][ncol] += val;
                    }
                }
            }
        }
        /* copy ways_next to ways for next iteration */
        for (int r = 0; r < 16; ++r)
            for (int c = 0; c < 16; ++c)
                ways[r][c] = ways_next[r][c];
    }

    /* Sum all ways for length n */
    seq_t res = 0;
    for (int p = 0; p < 16; ++p)
        for (int c = 0; c < 16; ++c)
            res += ways[p][c];

    return res;
}
