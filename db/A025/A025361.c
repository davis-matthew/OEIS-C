/*
 * A025361  Numbers that are the sum of 4 nonzero squares in exactly 5 ways.
 * https://oeis.org/A025361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 87, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025361: numbers that are the sum of 4 nonzero squares in exactly 5 unordered ways.
   The function a(idx_t n) returns the nth such number (a(1)=82). */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* static data for memoisation ------------------------------------------------- */
static seq_t  *terms = NULL;      /* collected terms */
static idx_t   terms_len = 0;     /* number of terms stored */
static idx_t   current_limit = 0; /* highest n value processed by recompute */

/* --------------------------------------------------------------------------- */
/* integer square root for idx_t (assumed to fit in uint64_t) */
static idx_t
isqrt_idx(idx_t n)
{
    idx_t r = (idx_t)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* recompute sequence up to limit (inclusive) */
static void
recompute_limit(idx_t limit)
{
    /* allocate helper arrays */
    uint32_t *c0 = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *c1 = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *c2 = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *c3 = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *c4 = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!c0 || !c1 || !c2 || !c3 || !c4) { /* allocation failed – give up */
        free(c0); free(c1); free(c2); free(c3); free(c4);
        return;
    }

    c0[0] = 1; /* base case: zero squares sum to 0 */

    /* iterate over each square value */
    idx_t max_root = isqrt_idx(limit);
    for (idx_t a = 1; a <= max_root; ++a) {
        idx_t sq = a * a;
        for (idx_t s = sq; s <= limit; ++s) {
            uint32_t add4 = c3[s - sq];
            if (add4) c4[s] += add4;
            uint32_t add3 = c2[s - sq];
            if (add3) c3[s] += add3;
            uint32_t add2 = c1[s - sq];
            if (add2) c2[s] += add2;
            uint32_t add1 = c0[s - sq];
            if (add1) c1[s] += add1;
        }
    }

    /* count how many numbers satisfy the condition */
    size_t cnt = 0;
    for (idx_t s = 1; s <= limit; ++s)
        if (c4[s] == 5)
            ++cnt;

    /* allocate terms array and store numbers */
    seq_t *new_terms = (seq_t*)malloc(cnt * sizeof(seq_t));
    size_t pos = 0;
    for (idx_t s = 1; s <= limit; ++s)
        if (c4[s] == 5)
            new_terms[pos++] = (seq_t)s;  /* cast to seq_t once */

    /* free old data */
    free(terms);
    free(c0); free(c1); free(c2); free(c3); free(c4);

    /* store new data */
    terms = new_terms;
    terms_len = (idx_t)cnt;
    current_limit = limit;
}

/* public function ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range */

    static bool initialized = false;

    /* initial construction */
    if (!initialized) {
        idx_t init_limit = 200000; /* starting guess sufficient for many queries */
        do {
            recompute_limit(init_limit);
            if (terms_len >= n) break;
            init_limit *= 2;
        } while (1);
        initialized = true;
    }

    /* extend if necessary */
    while (terms_len < n) {
        idx_t new_limit = current_limit * 2;
        recompute_limit(new_limit);
    }

    return terms[n - 1];
}
