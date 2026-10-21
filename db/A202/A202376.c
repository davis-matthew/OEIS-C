/*
 * A202376  Number of (n+2)X8 binary arrays avoiding patterns 001 and 111 in
 *          rows and columns.
 * https://oeis.org/A202376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 43, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counting (n+2)x8 binary arrays avoiding the horizontal and vertical
 * patterns 001 and 111.  The algorithm |
 * 1. Enumerate all binary rows of length 8 that avoid 001/111.
 * 2. For each pair of consecutive rows (p, q) pre‑compute the set of
 *    next rows that keep all vertical triples safe.
 * 3. Dynamic programming over rows:
 *    dp[(p, q)] = number of ways to reach rows ... , p, q.
 *    Transition to (q, r) using the precomputed mask.
 * 4. The total number of matrices for m = n+2 rows is the sum of
 *    all dp states after processing the last row.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static int R = 0;                 /* number of valid rows (≤ 64)        */
static uint8_t rows[64];          /* bit masks of the 34 valid rows     */
static uint64_t adj[64][64];      /* adj[pa][pb] is bitmask of rows c   */
static size_t S = 0;              /* S = R * R                          */

static void precompute(void)
{
    /* Step 1: enumerate all valid rows */
    for (uint16_t pat = 0; pat < 256; ++pat) {
        bool ok = true;
        for (int j = 0; j <= 5; ++j) {          /* check horizontal triples */
            int b2 = (pat >> (j + 2)) & 1;
            int b1 = (pat >> (j + 1)) & 1;
            int b0 = (pat >> j) & 1;
            if ((b2 == 0 && b1 == 0 && b0 == 1) ||
                (b2 == 1 && b1 == 1 && b0 == 1)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            rows[R++] = pat & 0xFFU;
        }
    }
    S = (size_t)R * (size_t)R;

    /* Step 2: build adjacency masks */
    for (int pa = 0; pa < R; ++pa) {
        for (int pb = 0; pb < R; ++pb) {
            uint64_t mask = 0;
            for (int pc = 0; pc < R; ++pc) {
                bool col_ok = true;
                for (int col = 0; col < 8; ++col) {
                    int x = (rows[pa] >> col) & 1;
                    int y = (rows[pb] >> col) & 1;
                    int z = (rows[pc] >> col) & 1;
                    if ((x == 0 && y == 0 && z == 1) ||
                        (x == 1 && y == 1 && z == 1)) {
                        col_ok = false;
                        break;
                    }
                }
                if (col_ok) {
                    mask |= ((uint64_t)1U << pc);
                }
            }
            adj[pa][pb] = mask;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) precompute();

    idx_t m = n + 2;                 /* number of rows */
    /* dp[(p,q)] holds count of ways to reach rows ... , p, q. */
    seq_t *dp   = (seq_t *)calloc(S, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(S, sizeof(seq_t));
    if (!dp || !next) return 0;     /* allocation failure */

    /* initialise for the first two rows: any pair is allowed */
    for (size_t id = 0; id < S; ++id) dp[id] = 1;

    /* process remaining rows */
    for (idx_t row = 3; row <= m; ++row) {
        /* zero next array */
        for (size_t id = 0; id < S; ++id) next[id] = 0;

        for (size_t id = 0; id < S; ++id) {
            seq_t cnt = dp[id];
            if (!cnt) continue;
            size_t pa = id / (size_t)R;
            size_t pb = id % (size_t)R;
            uint64_t mask = adj[pa][pb];
            while (mask) {
                uint64_t lowbit = mask & -mask;
                size_t pc = __builtin_ctzll(mask);
                next[pb*(size_t)R + pc] += cnt;
                mask ^= lowbit;
            }
        }
        /* swap dp and next */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    /* sum all final states */
    seq_t total = 0;
    for (size_t id = 0; id < S; ++id) total += dp[id];

    free(dp);
    free(next);
    return total;
}
