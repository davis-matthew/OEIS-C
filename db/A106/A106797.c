/*
 * A106797  Fixed point of the morphism 1 -> 1,1,1,1,2,2,3; 2 -> 4,1; 3 ->
 *          2,1,1,1; 4 -> 1,2,1 starting with a(0) = 1.
 * https://oeis.org/A106797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th symbol (0‑based) of the fixed point * of the
 *            morphism * 1 → 1,1,1,1,2,2,3 * 2 → 4,1 * 3 → 2,1,1,1 * 4 → 1,2,1
 *            * starting with 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Mapping tables: each row holds up to the maximum mapping length (7).
   Unused entries are zero.  MAP_LEN gives the actual non‑zero length for
   each symbol.  Symbols are 1‑based in the morphism but stored 0‑based
   in the arrays for direct indexing. */
static const int MAP[4][7] = {
    {1, 1, 1, 1, 2, 2, 3},   /* 1 → 1111223 */
    {4, 1, 0, 0, 0, 0, 0},   /* 2 → 41       */
    {2, 1, 1, 1, 0, 0, 0},   /* 3 → 2111      */
    {1, 2, 1, 0, 0, 0, 0}    /* 4 → 121       */
};
static const int MAP_LEN[4] = {7, 2, 4, 3};

/* Pre‑computed lengths of the expansion of each symbol after a
   certain number of substitution steps.  LEN[d][s] = |σ^d(s)|.
   The maximum depth required for n up to 10 000 is far below 64,
   but we allow 64 as a safe upper bound. */
#define MAX_DEPTH 64
static uint64_t LEN[MAX_DEPTH][4];
static int  COMPUTED_DEPTH = 0;   /* last depth for which LEN is valid */

/* Compute lengths up to the requested depth.  Since len[0][s] = 1 for
   all symbols, the transition is
       LEN[d][s] = Σ LEN[d-1][child] over the symbols produced by s. */
static void ensure_depth(int d)
{
    if (d <= COMPUTED_DEPTH)
        return;

    /* Initialise depth 0: length of any single symbol is 1. */
    if (COMPUTED_DEPTH == 0) {
        for (int s = 0; s < 4; ++s)
            LEN[0][s] = 1;
    }

    for (int depth = COMPUTED_DEPTH + 1; depth <= d; ++depth) {
        for (int s = 0; s < 4; ++s) {
            uint64_t sum = 0;
            for (int k = 0; k < MAP_LEN[s]; ++k) {
                int child = MAP[s][k];          /* 1‑based symbol */
                sum += LEN[depth - 1][child - 1];
            }
            LEN[depth][s] = sum;
        }
    }
    COMPUTED_DEPTH = d;
}

/* Main function: return a(n).  We find a depth where the root symbol
   1 expands to a word long enough to cover n, then descend the
   substitution tree. */
seq_t a(idx_t n)
{
    /* Ensure at least depth 0 is ready. */
    ensure_depth(0);

    /* Find minimal depth such that LEN[depth][0] > n. */
    int depth = 0;
    while (LEN[depth][0] <= n) {
        if (depth + 1 >= MAX_DEPTH)      /* safety guard – should never happen */
            break;
        ensure_depth(depth + 1);
        ++depth;
    }

    /* Descend from the root symbol (1). */
    int symbol = 1;                     /* 1‑based symbol */
    while (depth > 0) {
        for (int k = 0; k < MAP_LEN[symbol - 1]; ++k) {
            int child = MAP[symbol - 1][k];            /* 1‑based */
            uint64_t child_len = LEN[depth - 1][child - 1];
            if (n < (idx_t)child_len) {
                symbol = child;
                --depth;
                break;
            } else {
                n -= (idx_t)child_len;
            }
        }
    }
    return (seq_t)symbol;
}
