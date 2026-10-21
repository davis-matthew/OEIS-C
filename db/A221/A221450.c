/*
 * A221450  Number of 5 X n binary arrays with every 0 a horizontal or
 *          antidiagonal neighbor to some 1.
 * https://oeis.org/A221450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 5×n binary arrays in which every 0 has a
 * horizontal (left/right) or antidiagonal (upper‑right/lower‑left) neighbour
 * that is 1.  Computed by linear‑time dynamic programming over column masks.
 */

#include <stdint.h>

#define ROWS 5
#define MASKS (1 << ROWS)      /* 32 possible column masks  */
#define STATE (MASKS * MASKS)  /* 32×32 = 1024 DP states   */

/* DP for OEIS A221450 */
seq_t a(idx_t n)
{
    /* Pre‑computed allowed transitions for each state (prevprev, prev). */
    static uint32_t allowed[STATE];
    static int built = 0;
    if (!built) {
        for (int prevprev = 0; prevprev < MASKS; ++prevprev) {
            for (int prev = 0; prev < MASKS; ++prev) {
                uint32_t bits = 0;
                for (int cur = 0; cur < MASKS; ++cur) {
                    int ok = 1;
                    /* Check each row of column 'prev'. */
                    for (int r = 0; r < ROWS; ++r) {
                        if (prev & (1 << r)) continue;  /* already 1 */
                        int left = (prevprev >> r) & 1;               /* horizontal left */
                        if (r + 1 < ROWS) left |= (prevprev >> (r + 1)) & 1; /* lower‑left */
                        if (!left) {
                            int right = (cur >> r) & 1;                 /* horizontal right */
                            if (r > 0) right |= (cur >> (r - 1)) & 1;   /* upper‑right */
                            if (!right) { ok = 0; break; }
                        }
                    }
                    if (ok) bits |= (1u << cur);
                }
                allowed[prevprev * MASKS + prev] = bits;
            }
        }
        built = 1;
    }

    static seq_t dp[STATE];
    static seq_t next[STATE];

    /* Initialise DP: column 0, fictitious previous column of all zeros. */
    for (int i = 0; i < STATE; ++i) dp[i] = 0;
    for (int mask0 = 0; mask0 < MASKS; ++mask0)
        dp[mask0] = 1;                 /* state (prevprev=0, prev=mask0) */

    /* Process columns 1 … n‑1. */
    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < STATE; ++i) next[i] = 0;
        for (int s = 0; s < STATE; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            int prev = s & (MASKS - 1);              /* lower 5 bits = prev column */
            uint32_t bits = allowed[s];
            while (bits) {
                int cur = __builtin_ctz(bits);
                bits &= bits - 1;                    /* clear lowest set bit */
                int nextState = prev * MASKS + cur;  /* new (prevprev,prev) */
                next[nextState] += ways;
            }
        }
        for (int i = 0; i < STATE; ++i) dp[i] = next[i];
    }

    /* Final validation: zeros in last column must have a left neighbour
       (either horizontal or lower‑left from column n‑2). */
    seq_t ans = 0;
    for (int s = 0; s < STATE; ++s) {
        seq_t ways = dp[s];
        if (!ways) continue;
        int prevprev = s >> ROWS;             /* previous‑previous column */
        int prev     = s & (MASKS - 1);       /* last column */
        int ok = 1;
        for (int r = 0; r < ROWS; ++r) {
            if (prev & (1 << r)) continue;    /* already 1 */
            int left = (prevprev >> r) & 1;    /* horizontal left */
            if (r + 1 < ROWS) left |= (prevprev >> (r + 1)) & 1; /* lower‑left */
            if (!left) { ok = 0; break; }
        }
        if (ok) ans += ways;
    }
    return ans;
}
