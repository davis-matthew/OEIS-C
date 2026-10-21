/*
 * A234892  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A234892: number of (n+1)×3 arrays with 0..6 entries,
   no equal adjacent elements, and each 2×2 subblock having its
   diagonal sum differ from its anti‑diagonal sum by 1.
   The values are computed with a dynamic‑programming over row
   patterns (O(P²·n), with P = 252). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Number of admissible row patterns (a,b,c) with a!=b and b!=c.  */
#define P 252

/* Store all row patterns.  patterns[i][0..2] is pattern i.  */
static uint8_t patterns[P][3];

/* For each pattern i, list of patterns that may precede it vertically.  */
static uint16_t prev_list[P][P];
static uint8_t  prev_count[P];

/* DP buffers for counting arrays ending with a given pattern.  */
static seq_t *dp_prev = NULL;
static seq_t *dp_curr = NULL;

static int initialized = 0;

/* ------------------------------------------------------------------ */
/*  Pre–initialise patterns, transition lists and DP buffers.          */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (initialized) return;

    /* Build all admissible patterns. */
    size_t idx = 0;
    for (uint8_t a = 0; a <= 6; ++a) {
        for (uint8_t b = 0; b <= 6; ++b) if (b != a) {
            for (uint8_t c = 0; c <= 6; ++c) if (c != b) {
                patterns[idx][0] = a;
                patterns[idx][1] = b;
                patterns[idx][2] = c;
                ++idx;
            }
        }
    }

    /* Pre–compute vertical transition lists. */
    for (size_t prev = 0; prev < P; ++prev) {
        const uint8_t *p = patterns[prev];
        for (size_t curr = 0; curr < P; ++curr) {
            const uint8_t *c = patterns[curr];

            /* Vertical adjacency: columns must differ. */
            if (c[0] == p[0] || c[1] == p[1] || c[2] == p[2]) continue;

            /* Diagonal vs anti‑diagonal condition for the two 2×2 blocks. */
            int d0 = p[0] + c[1];
            int a0 = p[1] + c[0];
            int d1 = p[1] + c[2];
            int a1 = p[2] + c[1];
            if ( ((d0>a0)?(d0-a0):(a0-d0)) != 1 ) continue;
            if ( ((d1>a1)?(d1-a1):(a1-d1)) != 1 ) continue;

            /* Valid transition: record predecessor. */
            prev_list[curr][prev_count[curr]++] = (uint16_t)prev;
        }
    }

    /* Allocate DP buffers. */
    dp_prev = (seq_t *)calloc(P, sizeof(seq_t));
    dp_curr = (seq_t *)calloc(P, sizeof(seq_t));

    initialized = 1;
}

/* ------------------------------------------------------------------ */
/*  Public member function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();
    if (!initialized) return 0;      /* Allocation failure.  */
    if (n < 1) return 0;              /* Sequence defined for n≥1. */

    /* Each pattern contributes one array of height 1. */
    for (size_t i = 0; i < P; ++i) dp_prev[i] = 1;

    /* Apply n vertical transitions to reach height n+1. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero current layer. */
        for (size_t i = 0; i < P; ++i) dp_curr[i] = 0;

        /* Accumulate transitions. */
        for (size_t curr = 0; curr < P; ++curr) {
            uint8_t  count  = prev_count[curr];
            const uint16_t *pred = prev_list[curr];
            seq_t sum = 0;
            for (uint8_t k = 0; k < count; ++k) sum += dp_prev[pred[k]];
            dp_curr[curr] = sum;
        }

        /* Prepare for next step. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Sum over all ending patterns to obtain total arrays. */
    seq_t total = 0;
    for (size_t i = 0; i < P; ++i) total += dp_prev[i];
    return total;
}
