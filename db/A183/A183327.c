/*
 * A183327  Number of n X 6 binary arrays with each 1 adjacent to exactly two
 *          other 1's.
 * https://oeis.org/A183327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A183327 – Number of n×6 binary arrays with each 1 adjacent to exactly two other 1's.
    The values are computed by a linear‑time DP over pairs of consecutive rows.
    The DP state is (previous_row, current_row).  Transitions are precomputed once.
    The algorithm works for any seq_t type (fixed‑size unsigned integer or
    arbitrary‑precision big integer in C++).  The DP tables and cache are
    allocated with the appropriate constructors for C++ or with calloc for C.
    The function a(n) returns a(1)=1 as required by the offset.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define WIDTH   6
#define STATES  (1 << WIDTH)          /* 64 possible 6‑bit rows          */
#define STATE_COUNT (STATES * STATES) /* 4096 possible (up,mid) pairs    */

/* --------------------------------------------------------------------- */
/*                         State‑transition tables                         */
/* --------------------------------------------------------------------- */
static uint64_t allowed[STATE_COUNT];      /* mask of admissible low rows    */
static bool     final_ok[STATE_COUNT];     /* result valid when low = 0       */

/* --------------------------------------------------------------------- */
/*                           DP table allocation                           */
/* --------------------------------------------------------------------- */
static seq_t *dp = NULL, *nextdp = NULL;

/* --------------------------------------------------------------------- */
/*                         One‑time initialization                         */
/* --------------------------------------------------------------------- */
static void init(void)
{
    /* allocate DP buffers once */
#if defined(__cplusplus)
    dp = new seq_t[STATE_COUNT]();      /* value‑initialized to zero */
    nextdp = new seq_t[STATE_COUNT]();  /* value‑initialized to zero */
#else
    dp = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
    nextdp = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
#endif
    if (!dp || !nextdp) return;          /* out of memory – should not happen */

    /* initial state: before any rows, up = 0, mid = 0 */
    dp[0] = (seq_t)1;

    /* precompute allowed low rows and final_ok mask */
    for (int up = 0; up < STATES; ++up) {
        for (int mid = 0; mid < STATES; ++mid) {
            int id = (up << WIDTH) | mid;
            uint64_t mask = 0ULL;
            for (int low = 0; low < STATES; ++low) {
                /* check that every 1 in the mid row has exactly two neighbours
                   considering up, low, left & right.  0‑cells are ignored. */
                int neigh = 0;
                for (int col = 0; col < WIDTH; ++col) {
                    if (((mid >> col) & 1) == 0) continue;
                    int cnt = ((up   >> col) & 1)              /* up      */
                            + ((low  >> col) & 1)              /* down    */
                            + ((col > 0   ? (mid >> (col-1)) & 1 : 0)); /* left  */
                    cnt += ((col < WIDTH-1 ? (mid >> (col+1)) & 1 : 0)); /* right */
                    if (cnt != 2) { neigh = -1; break; }
                }
                if (neigh >= 0) mask |= (1ULL << low);
            }
            allowed[id] = mask;
            /* last row is valid when its low neighbour is absent (0) */
            int neigh_main = 0;
            for (int col = 0; col < WIDTH; ++col) {
                if (((mid >> col) & 1) == 0) continue;
                int cnt = ((up   >> col) & 1); /* up only, low=0 */
                cnt += ((col > 0   ? (mid >> (col-1)) & 1 : 0));
                cnt += ((col < WIDTH-1 ? (mid >> (col+1)) & 1 : 0));
                if (cnt != 2) { neigh_main = -1; break; }
            }
            final_ok[id] = (neigh_main >= 0);
        }
    }
}

/* --------------------------------------------------------------------- */
/*                        Cache for previously computed a(n)              */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;      /* array of computed a(n) values */
static size_t cache_cap = 0;     /* current allocated size of cache */
static idx_t  max_n    = 0;      /* highest n for which cache is valid */

/* Grow cache to at least size n+1 */
static void grow_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache;
#if defined(__cplusplus)
    new_cache = new seq_t[new_cap]();       /* value‑initialize to zero */
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    delete[] cache;
#else
    new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;                /* allocation failed – ignore */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*                          Main sequence function                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    if (!dp) init();                      /* first call: allocate tables */

    /* If value already cached, return it immediately */
    if (n <= max_n) return cache[n];

    /* Compute successive terms up to n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* reset nextdp */
        for (int i = 0; i < STATE_COUNT; ++i) nextdp[i] = 0;

        /* transition from each existing state */
        for (int id = 0; id < STATE_COUNT; ++id) {
            seq_t val = dp[id];
            if (val == 0) continue;
            int mid = id & (STATES - 1);
            uint64_t mask = allowed[id];
            while (mask) {
                uint64_t lowbit = mask & -mask;
                int low = __builtin_ctzll(mask);
                nextdp[(mid << WIDTH) | low] += val;
                mask ^= lowbit;
            }
        }

        /* swap dp and nextdp for next iteration */
        seq_t *tmp = dp;
        dp = nextdp;
        nextdp = tmp;

        /* sum states whose last row is final‑valid */
        seq_t ans = 0;
        for (int id = 0; id < STATE_COUNT; ++id)
            if (final_ok[id]) ans += dp[id];

        /* store result in cache */
        grow_cache(cur);
        cache[cur] = ans;
    }

    max_n = n;
    return cache[n];
}
