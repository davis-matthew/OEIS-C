/*
 * A207469  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 36, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP algorithm for A207469.
   Number of 5×n 0..1 arrays avoiding horizontal patterns 000 and 011
   and vertical patterns 010 and 101.  The function returns a(n)
   for every n ≥ 1 in O(n·16³) time.  */

/* Standard headers for the required utilities. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Build the list of valid 5‑bit columns (vertical constraints). */
    static int pattern_idx[32];
    static int patterns[16];
    static int N = 0;
    static bool trans[16][16][16];
    static bool initialized = 0;

    if (!initialized) {
        memset(pattern_idx, 0xff, sizeof pattern_idx);   /* -1 for all entries */
        N = 0;
        for (int pat = 0; pat < 32; ++pat) {
            bool ok = true;
            for (int r = 0; r <= 2; ++r) {
                int bits =
                    ((pat >> r)     & 1) |
                    (((pat >> (r+1)) & 1) << 1) |
                    (((pat >> (r+2)) & 1) << 2);
                if (bits == 2 || bits == 5) {   /* 010 or 101 forbidden */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                pattern_idx[pat] = N;
                patterns[N++] = pat;
            }
        }

        /* Pre‑compute transitions of triples of columns. */
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                for (int k = 0; k < N; ++k) {
                    bool ok = true;
                    for (int r = 0; r < 5; ++r) {
                        int a = (patterns[i] >> r) & 1;
                        int b = (patterns[j] >> r) & 1;
                        int c = (patterns[k] >> r) & 1;
                        int triple = (a << 2) | (b << 1) | c;   /* 000 or 011 forbidden */
                        if (triple == 0 || triple == 3) { ok = false; break; }
                    }
                    trans[i][j][k] = ok;
                }

        initialized = true;
    }

    /* Base cases: n=1 and n=2. */
    if (n == 1) return (seq_t)N;
    if (n == 2) return (seq_t)N * (seq_t)N;

    /* DP arrays: freq[i][j] = number of ways ending with columns i and j. */
    seq_t freq[16][16];
    seq_t newfreq[16][16];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            freq[i][j] = 1;          /* every pair is fine for n=2 */

    for (idx_t step = 3; step <= n; ++step) {
        /* reset newfreq */
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                newfreq[i][j] = 0;

        /* transition to next column k */
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                seq_t val = freq[i][j];
                if (!val) continue;
                for (int k = 0; k < N; ++k) {
                    if (trans[i][j][k])
                        newfreq[j][k] += val;
                }
            }
        }
        /* copy newfreq back to freq */
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                freq[i][j] = newfreq[i][j];
    }

    /* Sum all ending pairs. */
    seq_t ans = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            ans += freq[i][j];

    return ans;
}
