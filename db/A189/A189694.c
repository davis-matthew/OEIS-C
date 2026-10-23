/*
 * A189694  Number of nX7 binary arrays without the pattern 0 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A189694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the number of n×7 binary arrays that contain
    no 0‑0‑1 pattern along any vertical, diagonal, or antidiagonal
    line.  It uses a dynamic‑programming recurrence over pairs of
    consecutive rows; the transition graph is pre‑computed once
    and re‑used for every call.  The DP runs in O(n · |states|) time
    and satisfies the memory and performance requirements. */

#include <stdint.h>
#include <stdlib.h>

#define MASK7 0x7F  /* 7‑bit mask */

/* fast popcount for an 8‑bit value (used only in C*/ 
static inline int popcnt8(uint8_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcount((int)x);
#else
    int c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
#endif
}

/* ------- Transition pre‑computation ------- */
static int            *trans_start;   /* first index in trans_next for each state   */
static int            *trans_len;     /* number of transitions out of each state   */
static uint8_t        *trans_next;     /* concatenated lists of possible next rows  */
static int             nstates;        /* number of states = 2^7 × 2^7 */

/* ------ DP buffers -------- */
static seq_t *dp_curr;   /* counts for the current layer of states   */
static seq_t *dp_next;   /* temporary buffer for the next layer    */

/* ------ Cache of results ------- */
static seq_t *cache;      /* cache[i] = a(i) for 0 ≤ i ≤ cache_size */
static idx_t  cache_size; /* largest n for which cache is filled      */

/* Forward declaration of initializer */
static void init(void);

/* ------------------------------- */
/*  Main sequence function          */
/* ------------------------------- */
seq_t a(idx_t n)
{
    /* Perform one‑time initialization */
    static int initialized = 0;
    if (!initialized) {
        init();
        initialized = 1;
    }

    if (n == 0ULL) return 0;          /* index 0 is unused, return 0 */

    /* Return cached value if available */
    if (n <= cache_size) return cache[n];

    /* Extend DP up to the requested n, filling the cache */
    for (idx_t step = cache_size + 1; step <= n; ++step) {
        /* set dp_next to zero */
        for (int i = 0; i < nstates; ++i) dp_next[i] = 0;

        /* propagate from current states to next states */
        for (int p = 0; p < nstates; ++p) {
            seq_t val = dp_curr[p];
            if (val == 0) continue;
            int curr = p & 127;          /* lower 7 bits = current row */

            int start = trans_start[p];
            int len   = trans_len[p];
            for (int k = 0; k < len; ++k) {
                uint8_t nxt = trans_next[start + k];
                int idx = (curr << 7) | nxt;
                dp_next[idx] += val;
            }
        }

        /* sum over all states for this n */
        seq_t sum = 0;
        for (int i = 0; i < nstates; ++i) sum += dp_next[i];

        /* grow cache if necessary */
        if (step > cache_size) {
            cache = (seq_t *)realloc(cache, (step + 1) * sizeof(seq_t));
        }
        cache[step] = sum;
        cache_size  = step;

        /* swap dp buffers for next iteration */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    return cache[n];
}

/* ------------------------------------------- */
/*   Initializer: build transition tables & DP  */
/* ------------------------------------------- */
static void init(void)
{
    nstates = 128 * 128; /* 2^7 × 2^7 */

    /* First pass: compute transition counts and total number of entries */
    trans_start = (int *)calloc(nstates, sizeof(int));
    trans_len   = (int *)calloc(nstates, sizeof(int));
    int total_entries = 0;

    for (int prev = 0; prev < 128; ++prev) {
        for (int curr = 0; curr < 128; ++curr) {
            uint8_t vp = (uint8_t)prev;
            uint8_t vc = (uint8_t)curr;

            /* Compute bits forced to be zero in the next row */
            uint8_t forced = (~vp) & (~vc) & MASK7;                 /* vertical */

            /* diagonal 0 0 1 */
            for (int j = 0; j <= 4; ++j) {
                if (!( (vp >> j) & 1 ) && !( (vc >> (j + 1)) & 1 ))
                    forced |= (1U << (j + 2));
            }

            /* antidiagonal 0 0 1 */
            for (int j = 2; j <= 6; ++j) {
                if (!( (vp >> j) & 1 ) && !( (vc >> (j - 1)) & 1 ))
                    forced |= (1U << (j - 2));
            }

            uint8_t free_mask = (~forced) & MASK7;
            int free_bits = popcnt8(free_mask);
            int len = 1 << free_bits;          /* number of possible nxt rows */

            int idx = (prev << 7) | curr;
            trans_len[idx]  = len;
            trans_start[idx] = total_entries;
            total_entries += len;
        }
    }

    /* Allocate storage for all transition lists */
    trans_next = (uint8_t *)malloc(total_entries * sizeof(uint8_t));

    /* Second pass: fill the lists */
    for (int prev = 0; prev < 128; ++prev) {
        for (int curr = 0; curr < 128; ++curr) {
            uint8_t vp = (uint8_t)prev;
            uint8_t vc = (uint8_t)curr;

            uint8_t forced = (~vp) & (~vc) & MASK7;
            for (int j = 0; j <= 4; ++j) {
                if (!( (vp >> j) & 1 ) && !( (vc >> (j + 1)) & 1 ))
                    forced |= (1U << (j + 2));
            }
            for (int j = 2; j <= 6; ++j) {
                if (!( (vp >> j) & 1 ) && !( (vc >> (j - 1)) & 1 ))
                    forced |= (1U << (j - 2));
            }

            uint8_t free_mask = (~forced) & MASK7;
            int idx = (prev << 7) | curr;
            int start = trans_start[idx];
            int len   = trans_len[idx];

            /* enumerate all subsets of free_mask */
            uint8_t sub = free_mask;
            for (int k = 0; k < len; ++k) {
                trans_next[start + k] = sub;
                if (sub == 0) break;
                sub = (sub - 1) & free_mask;
            }
        }
    }

    /* Allocate DP buffers */
    dp_curr = (seq_t *)malloc(nstates * sizeof(seq_t));
    dp_next = (seq_t *)malloc(nstates * sizeof(seq_t));
    for (int i = 0; i < nstates; ++i) dp_curr[i] = 1; /* base for n = 2 */

    /* Allocate cache and store base values a(1) = 128, a(2) = 16384 */
    cache_size = 2;
    cache = (seq_t *)malloc(3 * sizeof(seq_t));
    cache[0] = (seq_t)0;                    /* unused */
    cache[1] = (seq_t)128;                  /* 2^7 */
    cache[2] = (seq_t)16384;                /* 2^14 */
}
