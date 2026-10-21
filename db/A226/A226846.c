/*
 * A226846  Number of nX2 (0,1,2) arrays of permanents of 2X2 subblocks of
 *          some (n+1)X3 binary array.
 * https://oeis.org/A226846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct n×2 arrays of permanents (0,1,2)
 *            * of 2×2 subblocks of some (n+1)×3 binary array. * The values
 *            are obtained by constructing a DFA where states are subsets * of
 *            the 8 possible 3‑bit rows of the binary array. * For each symbol
 *            (permanent pair) 0..8 we precompute the t...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool transition_inited = false;
static uint8_t trans[256][9];

/* Precompute the transition table trans[mask][sym] = new_mask. */
static void init_transition(void)
{
    if (transition_inited) return;
    transition_inited = true;

    /* Output symbol for transition from row s to row t:
     *  (p0,p1) where p0= B[i][0]*B[i+1][1] + B[i][1]*B[i+1][0]
     *       p1= B[i][1]*B[i+2][0] + B[i][2]*B[i+1][1]
     *  Symbol = p0*3 + p1  (0..8)   */
    uint8_t out[8][8];
    for (int s = 0; s < 8; ++s) {
        int b0 = (s >> 0) & 1;
        int b1 = (s >> 1) & 1;
        int b2 = (s >> 2) & 1;
        for (int t = 0; t < 8; ++t) {
            int c0 = (t >> 0) & 1;
            int c1 = (t >> 1) & 1;
            int c2 = (t >> 2) & 1;
            int p0 = b0 * c1 + b1 * c0;
            int p1 = b1 * c2 + b2 * c1;
            out[s][t] = (uint8_t)(p0 * 3 + p1); /* value 0..8 */
        }
    }

    for (int mask = 0; mask < 256; ++mask) {
        for (int sym = 0; sym < 9; ++sym) {
            uint8_t dest = 0;
            int m = mask;
            while (m) {
                int s = __builtin_ctz(m);
                m &= m - 1;
                for (int t = 0; t < 8; ++t) {
                    if (out[s][t] == sym) dest |= (1U << t);
                }
            }
            trans[mask][sym] = dest;
        }
    }
}

/* DP state arrays and cache. */
static seq_t *dp_cur = NULL;   /* current counts for words of current length */
static seq_t *dp_next = NULL;  /* buffer for next length */
static seq_t *a_cache = NULL;  /* cached a(n) values, index 0 -> a(1) */
static size_t a_cache_cap = 0;
static size_t a_cache_size = 0; /* number of stored terms */

/* Helper: ensure cache can hold up to new_cap entries. */
static void grow_a_cache(size_t new_cap)
{
    if (new_cap <= a_cache_cap) return;
    size_t cap = new_cap;
    if (cap < 16) cap = 16;
    seq_t *tmp = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
    if (!tmp) exit(1); /* allocation failure */
    a_cache = tmp;
    a_cache_cap = cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    init_transition();

    /* First call: allocate DP arrays. */
    if (!dp_cur) {
        dp_cur = (seq_t *)calloc(256, sizeof(seq_t));
        dp_next = (seq_t *)calloc(256, sizeof(seq_t));
        if (!dp_cur || !dp_next) exit(1);
        dp_cur[255] = (seq_t)1; /* all 8 states reachable initially */
    }

    /* Extend cache/DP up to requested n. */
    for (size_t len = a_cache_size + 1; len <= n; ++len) {
        /* compute dp_next from dp_cur */
        for (int m = 0; m < 256; ++m) dp_next[m] = (seq_t)0;

        for (int mask = 0; mask < 256; ++mask) {
            seq_t ways = dp_cur[mask];
            if (!ways) continue;
            for (int sym = 0; sym < 9; ++sym) {
                uint8_t dest = trans[mask][sym];
                if (dest == 0) continue;          /* leads to empty set => invalid word */
                dp_next[dest] += ways;
            }
        }

        /* Sum all non‑empty subsets to get a(len). */
        seq_t val = (seq_t)0;
        for (int m = 1; m < 256; ++m) val += dp_next[m];

        /* Store in cache. */
        grow_a_cache(len);
        a_cache[len - 1] = val;   /* a_cache[0] corresponds to n=1 */

        /* Prepare for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    a_cache_size = n;
    return a_cache[n - 1];
}
