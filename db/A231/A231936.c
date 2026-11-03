/*
 * A231936  Number of nX3 0..3 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 178 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic‐programming algorithm for A231936.
   The function counts n×3 arrays with entries 0..3 in which every element
   is NOT less than a strict majority of its horizontal, diagonal and
   antidiagonal neighbors.  The algorithm precomputes all allowed row
   transitions and then runs a DP over the rows; it is O(n·64²). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* check whether a row is valid when its neighbours are given.  If a
   neighbour row does not exist the corresponding flag is false and
   that side is ignored. */
static bool
row_valid(const uint8_t row[3], const uint8_t prev[3], const uint8_t next[3],
          bool has_prev, bool has_next)
{
    for (int c = 0; c < 3; ++c) {
        int v = row[c];
        int neigh = 0, great = 0;

        /* horizontal */
        if (c > 0) { neigh++; if (row[c-1] > v) great++; }
        if (c < 2) { neigh++; if (row[c+1] > v) great++; }

        /* diagonal and antidiagonal from prev row */
        if (has_prev) {
            if (c > 0) { neigh++; if (prev[c-1] > v) great++; }
            if (c < 2) { neigh++; if (prev[c+1] > v) great++; }
        }

        /* diagonal and antidiagonal from next row */
        if (has_next) {
            if (c > 0) { neigh++; if (next[c-1] > v) great++; }
            if (c < 2) { neigh++; if (next[c+1] > v) great++; }
        }

        if (great * 2 > neigh)   /* a strict majority > than v ? */
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Global tables are built only once */
    static bool initialized = false;

    /* encode all 64 possible rows (3 columns, values 0..3) */
    static uint8_t row[64][3];

    /* precomputed masks/tables */
    static uint64_t first_pair_mask[64];          /* first row → allowed second rows */
    static uint64_t transition_mask[64][64];      /* (prev,curr) → allowed next rows */
    static unsigned char last_valid[64][64];      /* (prev,curr) valid with no next row */
    static unsigned char row_own_valid[64];       /* row valid with no neighbours (n==1) */

    /* DP arrays */
    static seq_t dp[4096];          /* state (prev,curr)  */
    static seq_t dp_next[4096];     /* temporary for transitions */

    /* ------------------------------------------------------------ */
    /* initialise lookup tables once                                         */
    if (!initialized) {
        /* encode rows */
        for (int r = 0; r < 64; ++r) {
            row[r][0] = (r >> 0) & 3;
            row[r][1] = (r >> 2) & 3;
            row[r][2] = (r >> 4) & 3;
        }

        /* compute row_own_valid, first_pair_mask, transition_mask, last_valid */
        for (int r = 0; r < 64; ++r) {
            row_own_valid[r] = row_valid(row[r], row[r], row[r], false, false);

            /* first row → second row */
            uint64_t m = 0;
            for (int s = 0; s < 64; ++s)
                if (row_valid(row[r], row[r], row[s], false, true))
                    m |= ((uint64_t)1 << s);
            first_pair_mask[r] = m;

            for (int curr = 0; curr < 64; ++curr) {
                last_valid[r][curr] = row_valid(row[curr], row[r], row[curr], true, false);

                uint64_t m2 = 0;
                for (int next = 0; next < 64; ++next)
                    if (row_valid(row[curr], row[r], row[next], true, true))
                        m2 |= ((uint64_t)1 << next);
                transition_mask[r][curr] = m2;
            }
        }
        initialized = true;
    }

    /* ------------------------------------------------------------ */
    /* handle n==1 separately                                         */
    if (n == 1) {
        seq_t ans = 0;
        for (int i = 0; i < 64; ++i)
            if (row_own_valid[i]) ans += (seq_t)1;
        return ans;
    }

    /* ------------------------------------------------------------ */
    /* clear DP array for this call (previous results may remain) */
    for (int i = 0; i < 4096; ++i) dp[i] = 0;

    /* initialize for rows 1 and 2 */
    for (int first = 0; first < 64; ++first) {
        uint64_t mask = first_pair_mask[first];
        while (mask) {
            int second = __builtin_ctzll(mask);     /* index of set bit */
            dp[(first << 6) | second] = (seq_t)1;
            mask &= mask - 1;
        }
    }

    /* DP for rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear dp_next */
        for (int i = 0; i < 4096; ++i) dp_next[i] = (seq_t)0;

        for (int i = 0; i < 4096; ++i) {
            seq_t val = dp[i];
            if (!val) continue;

            int prev = i >> 6;
            int curr = i & 63;
            uint64_t mask = transition_mask[prev][curr];
            while (mask) {
                int next = __builtin_ctzll(mask);
                dp_next[(curr << 6) | next] += val;
                mask &= mask - 1;
            }
        }

        /* swap dp and dp_next, and zero dp_next for next round */
        for (int i = 0; i < 4096; ++i) {
            dp[i] = dp_next[i];
            dp_next[i] = (seq_t)0;
        }
    }

    /* sum over states where last row is valid (no next neighbour) */
    seq_t ans = 0;
    for (int i = 0; i < 4096; ++i) {
        seq_t val = dp[i];
        if (!val) continue;
        int prev = i >> 6;
        int curr = i & 63;
        if (last_valid[prev][curr])
            ans += val;
    }
    return ans;
}
