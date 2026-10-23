/*
 * A102800  Let f(n) = A104246(n) be the minimal number of nonzero tetrahedral
 *          numbers that add to n; sequence gives numbers n for which f(n) <=
 *          4.
 * https://oeis.org/A102800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A102800
   a(n) = the nth integer k such that the minimal number of non‑zero tetrahedral
   numbers summing to k is <= 4.  The minimal number f(k) = A104246(k).
   This routine builds the sequence once and returns terms on demand.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool seq_initialized = false;
static seq_t *seq_values = NULL;
static size_t seq_size = 0;

/* Build the sequence once (at most up to 500000). */
static void build_sequence(void)
{
    const idx_t LIMIT = 500000;          /* enough for the first 10 000 terms */
    const uint8_t MAX_REP = 5;           /* store counts up to 5 (>=4) */

    /* Allocate array for minimal counts (0 … 4, capped at 5). */
    uint8_t *min_rep = (uint8_t*)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!min_rep) return;                /* allocation failed */

    /* Generate tetrahedral numbers <= LIMIT. */
    const size_t MAX_TETRA = 200;        /* ample for LIMIT < 500000 */
    idx_t *tetra = (idx_t*)malloc(MAX_TETRA * sizeof(idx_t));
    size_t tcnt = 0;
    idx_t k = 1;
    while (1) {
        idx_t t = k * (k + 1) * (k + 2) / 6;
        if (t > LIMIT) break;
        if (tcnt == MAX_TETRA) {         /* unlikely to happen */
            tcnt += 50;
            tetra = (idx_t*)realloc(tetra, tcnt * sizeof(idx_t));
        }
        tetra[tcnt++] = t;
        ++k;
    }

    /* Dynamic programming for minimal representations. */
    min_rep[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        uint8_t best = MAX_REP;
        for (size_t j = 0; j < tcnt; ++j) {
            idx_t t = tetra[j];
            if (t > i) break;
            uint8_t cand = min_rep[i - t] + 1;
            if (cand < best) best = cand;
            if (best == 1) break;
        }
        min_rep[i] = best;
    }

    /* Count how many numbers satisfy the condition. */
    size_t count = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (min_rep[i] <= 4) ++count;
    }

    /* Allocate sequence storage and fill it. */
#ifdef __cplusplus
    seq_values = new seq_t[count];
#else
    seq_values = (seq_t*)malloc(count * sizeof(seq_t));
#endif
    size_t pos = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (min_rep[i] <= 4) {
#ifdef __cplusplus
            seq_values[pos++] = seq_t(i);
#else
            seq_values[pos++] = (seq_t)i;
#endif
        }
    }
    seq_size = count;

    /* Clean up. */
    free(min_rep);
    free(tetra);
    seq_initialized = true;
}

seq_t a(idx_t n)
{
    if (!seq_initialized) build_sequence();
    if (n == 0 || n > seq_size) return (seq_t)0;   /* undefined but safe */
    return seq_values[n - 1];                      /* 1‑based offset */
}
