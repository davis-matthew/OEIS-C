/*
 * A207490  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 23, u64 n <= 52, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A207490:  a(n) = number of 5×n binary arrays with the
    forbidden horizontal triples (0,0,0) and (0,1,1), and forbidden
    vertical triples (0,1,1) and (1,1,0).
    The routine uses a transfer‑matrix DP over columns.  */

/*  Standard includes for all compilers. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/*  Helper: per‑column vertical feasibility.
    There are 2^5 = 32 possible 5‑bit columns. */
static bool vertical_ok[32];
static uint32_t trans[32][32];   /* bitset of allowed third columns   */
static seq_t vertical_cnt = 0;   /* number of columns satisfying vertical rule */

/*  Initialise the vertical_ok table and the transition matrix.  */
static void init_transitions(void)
{
    static bool ready = false;
    if (ready) return;
    ready = true;

    for (int m = 0; m < 32; ++m) {     /* test each 5‑bit mask    */
        bool ok = true;
        for (int r = 0; r <= 2; ++r) { /* scan all 3‑row windows */
            int b0 = (m >> r)     & 1;
            int b1 = (m >> (r+1)) & 1;
            int b2 = (m >> (r+2)) & 1;
            if ((b0==0 && b1==1 && b2==1) || (b0==1 && b1==1 && b2==0)) {
                ok = false;
                break;
            }
        }
        vertical_ok[m] = ok;
        if (ok) ++vertical_cnt;
    }

    for (int a = 0; a < 32; ++a) {
        if (!vertical_ok[a]) continue;
        for (int b = 0; b < 32; ++b) {
            if (!vertical_ok[b]) continue;
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c) {
                if (!vertical_ok[c]) continue;
                bool ok = true;
                for (int r = 0; r < 5; ++r) {
                    int ba = (a >> r) & 1;
                    int bb = (b >> r) & 1;
                    int bc = (c >> r) & 1;
                    if ((ba==0 && bb==0 && bc==0) || (ba==0 && bb==1 && bc==1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) mask |= (1u << c);
            }
            trans[a][b] = mask;
        }
    }
}

/*  Cache for already computed values.  */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/*  Allocates an array of size new_cap (zeroed).  */
static seq_t *alloc_cache(idx_t new_cap)
{
    seq_t *p = (seq_t*)calloc((size_t)new_cap, sizeof(seq_t));
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* unused, offset is 1 */

    init_transitions();

    /* Expand cache if necessary.  */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = alloc_cache(new_cap);
        if (cache != NULL) {
            /* preserve existing cached values */
            for (idx_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return a memoized result.  */
    if (cache[n] != 0) return cache[n];

    /* Base cases.  */
    if (n == 1) {
        cache[1] = vertical_cnt;
        return cache[1];
    }
    if (n == 2) {
        cache[2] = (seq_t)vertical_cnt * (seq_t)vertical_cnt;
        return cache[2];
    }

    /* DP arrays: dp_cur[a][b] = ways ending with last two columns
       a,b (positions n-1, n).  */
    seq_t dp_cur[32][32] = {0};
    seq_t dp_next[32][32] = {0};

    /* Initialise for columns 1 and 2.  */
    for (int a = 0; a < 32; ++a)
        if (vertical_ok[a])
            for (int b = 0; b < 32; ++b)
                if (vertical_ok[b])
                    dp_cur[a][b] = 1;

    /* Extend columns 3 … n.  */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero dp_next */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_next[a][b] = 0;

        for (int a = 0; a < 32; ++a) if (vertical_ok[a]) {
            for (int b = 0; b < 32; ++b) if (vertical_ok[b]) {
                seq_t ways = dp_cur[a][b];
                if (!ways) continue;
                uint32_t mask = trans[a][b];
                uint32_t bits = mask;
                while (bits) {
                    uint32_t lsb = bits & -bits;
                    int c = __builtin_ctz(bits);
                    dp_next[b][c] += ways;
                    bits -= lsb;
                }
            }
        }

        /* Copy dp_next → dp_cur for next iteration */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_cur[a][b] = dp_next[a][b];
    }

    /* Sum all final states.  */
    seq_t total = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            total += dp_cur[a][b];

    cache[n] = total;
    return total;
}
