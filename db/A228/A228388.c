/*
 * A228388  Number of nX6 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally or vertically.
 * https://oeis.org/A228388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over rows with transfer matrix, enforcing no adjacent
   1s horizontally or vertically and fixing the top‑left entry to 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Number of patterns of 6 bits with no adjacent 1s */
#define NUM_PATTERNS 21

/* Storage for patterns and transition table */
static uint8_t  patterns[NUM_PATTERNS];
static uint8_t  trans[NUM_PATTERNS][NUM_PATTERNS];

/* One‑time initialization flag */
static bool patterns_initialized = false;

/* Initialise patterns and transition matrix */
static void init_patterns(void)
{
    if (patterns_initialized)
        return;

    unsigned int k = 0;
    /* Enumerate all 6‑bit masks with no horizontal adjacencies */
    for (uint8_t mask = 0; mask < 64; ++mask) {
        bool ok = true;
        for (int i = 0; i < 5 && ok; ++i) {
            if ((mask & (1u << i)) && (mask & (1u << (i + 1))))
                ok = false;
        }
        if (ok) {
            patterns[k++] = mask;
        }
    }

    /* Build transition matrix: patterns may follow each other iff
       they have no vertical adjacency (no common 1‑bit). */
    for (int i = 0; i < NUM_PATTERNS; ++i) {
        for (int j = 0; j < NUM_PATTERNS; ++j) {
            trans[i][j] = ((patterns[i] & patterns[j]) == 0) ? 1 : 0;
        }
    }

    patterns_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n = 0, return 0 safely */
        return (seq_t)0;

    init_patterns(); /* ensure tables are ready */

    /* DP arrays for two consecutive rows */
    seq_t dp_prev[NUM_PATTERNS] = {0};
    seq_t dp_curr[NUM_PATTERNS] = {0};

    /* Initialize first row: only patterns with top‑left (bit 0) set are allowed */
    for (int i = 0; i < NUM_PATTERNS; ++i) {
        dp_prev[i] = (patterns[i] & 1u) ? (seq_t)1 : (seq_t)0;
    }

    /* If only one row, sum and return */
    if (n == 1) {
        seq_t sum = 0;
        for (int i = 0; i < NUM_PATTERNS; ++i)
            sum += dp_prev[i];
        return sum;
    }

    /* Iterate rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int q = 0; q < NUM_PATTERNS; ++q) {
            seq_t s = 0;
            for (int p = 0; p < NUM_PATTERNS; ++p) {
                if (trans[p][q])
                    s += dp_prev[p];
            }
            dp_curr[q] = s;
        }
        /* Prepare for next iteration */
        for (int q = 0; q < NUM_PATTERNS; ++q)
            dp_prev[q] = dp_curr[q];
    }

    /* Final sum over the last row */
    seq_t sum = 0;
    for (int i = 0; i < NUM_PATTERNS; ++i)
        sum += dp_prev[i];
    return sum;
}
