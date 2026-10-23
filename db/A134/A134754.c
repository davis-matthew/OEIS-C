/*
 * A134754  Minimal number such that all greater numbers can be written as
 *          sums of squares >1 in more than n ways.
 * https://oeis.org/A134754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* formula: a(n) = maximum m such that A078134(j) <= n for all j > m */
/* implementation: compute the number of partitions of j into squares
   (squares > 1) by a bounded DP; for each n <= MAX_N_GLOBAL keep the
   largest j with a count <= n. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* constants -------------------------------------------------------------- */
#define MAX_N_GLOBAL 20000     /* largest index we pre‑compute                */
#define J_LIMIT      200000    /* upper bound for the DP, must be >= max j    */

/* cached data ------------------------------------------------------------ */
static seq_t *A_table = NULL;   /* a(n) for n = 0 .. MAX_N_GLOBAL           */
static uint32_t *ways  = NULL;  /* number of partitions of j into squares     */
static bool ready = false;

/* precompute the DP table ----------------------------------------------- */
static void precompute(void)
{
    if (ready) return;
    ready = true;

    /* build list of squares 4,9,16,… up to J_LIMIT                      */
    size_t sqcnt = 0;
    for (idx_t k = 2; (k * k) <= (idx_t)J_LIMIT; ++k) ++sqcnt;

    uint32_t *squares = (uint32_t*)malloc(sqcnt * sizeof(uint32_t));
    size_t s = 0;
    for (idx_t k = 2; (k * k) <= (idx_t)J_LIMIT; ++k)
        squares[s++] = (uint32_t)(k * k);

    /* DP for partitions into squares, capped at MAX_N_GLOBAL+1          */
    const uint32_t cap = (uint32_t)MAX_N_GLOBAL + 1;
    ways = (uint32_t*)calloc(J_LIMIT + 1, sizeof(uint32_t));
    ways[0] = 1; /* empty sum */

    for (size_t i = 0; i < sqcnt; ++i) {
        uint32_t sq = squares[i];
        for (idx_t j = sq; j <= (idx_t)J_LIMIT; ++j) {
            uint32_t prev = ways[j - sq];
            uint32_t newv = ways[j] + prev;
            if (newv > cap) newv = cap;
            ways[j] = newv;
        }
    }
    free(squares);

    /* last_index[c] = largest j with ways[j] == c  (c <= MAX_N_GLOBAL)   */
    uint32_t *last_index = (uint32_t*)calloc(MAX_N_GLOBAL + 1, sizeof(uint32_t));
    for (idx_t j = 0; j <= (idx_t)J_LIMIT; ++j) {
        uint32_t c = ways[j];
        if (c <= (uint32_t)MAX_N_GLOBAL)
            last_index[c] = (uint32_t)j;
    }

    /* build a(n) table: a[n] = max{ j | ways[j] <= n }                    */
    A_table = (seq_t*)malloc((MAX_N_GLOBAL + 1) * sizeof(seq_t));
    uint32_t running_max = 0;
    for (size_t n = 0; n <= (size_t)MAX_N_GLOBAL; ++n) {
        if (last_index[n] > running_max)
            running_max = last_index[n];
        A_table[n] = (seq_t)running_max;
    }

    free(last_index);
}

/* public accessor -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!ready) precompute();

    if (n <= (idx_t)MAX_N_GLOBAL)
        return A_table[(size_t)n];

    /* fallback (unlikely for n <= 10000) */
    seq_t last_bad = 0;
    for (idx_t j = 0; j <= (idx_t)J_LIMIT; ++j)
        if (ways[j] <= (uint32_t)n)
            last_bad = (seq_t)j;

    return last_bad;
}
