/*
 * A089936  Number of 5 X n matrices with entries {0,1} without adjacent 0's
 *          in any row or column. 5th row of A089934.
 * https://oeis.org/A089936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary matrices with no two * adjacent 0’s
 *            in any row or column. Computed by a transfer‑matrix * dynamic
 *            program over 5‑bit column masks. See comment above for * an
 *            explanation
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
 
#define ROWS       5
#define STATES    (1 << ROWS)
#define ZERO_MASK 0x1F   /* bits 0..4 set */
 
static int  valid_vert[STATES];
static int  trans[STATES][STATES];
 
static seq_t dp_prev[STATES];
static seq_t dp_next[STATES];
 
static seq_t *answers = NULL;        /* 1‑based: answers[1] = a(1) */
static idx_t  ans_alloc = 0;         /* allocated length (>= ans_count+1) */
static idx_t  ans_count = 0;         /* number of terms already computed */
 
static int tables_initialized = 0;
 
/* initialise admissibility tables */
static void
init_tables(void)
{
    int mask, zmask;
 
    /* vertical admissibility: no two consecutive zeros */
    for (mask = 0; mask < STATES; ++mask) {
        zmask = (~mask) & ZERO_MASK;
        valid_vert[mask] = ( (zmask & (zmask << 1)) == 0 );
    }
 
    /* adjacency matrix: horizontal constraint */
    for (int i = 0; i < STATES; ++i) {
        for (int j = 0; j < STATES; ++j) {
            if (!valid_vert[i] || !valid_vert[j]) {
                trans[i][j] = 0;
                continue;
            }
            /* horizontal adjacency: cannot have a zero in the same row
             * in both columns.  Since ~ may have bits beyond 5, mask
             * them to 0..4 first. */
            int zi = (~i) & ZERO_MASK;
            int zj = (~j) & ZERO_MASK;
            trans[i][j] = ( (zi & zj) == 0 );
        }
    }
 
    /* answers array initial allocation */
    ans_alloc = 16;
    answers = (seq_t*)malloc((size_t)ans_alloc * sizeof(seq_t));
    if (answers == NULL) abort();
    answers[0] = (seq_t)0;          /* dummy entry for 1‑based indexing */
 
    tables_initialized = 1;
}
 
/* ensure answers array can hold indices up to n */
static void
ensure_answers_size(idx_t n)
{
    if ((size_t)n + 1 <= ans_alloc)
        return;
    size_t newsize = ans_alloc * 2;
    if (newsize < (size_t)n + 1)
        newsize = (size_t)n + 1;
    seq_t *newptr = (seq_t*)realloc(answers, newsize * sizeof(seq_t));
    if (newptr == NULL) abort();
    answers = newptr;
    ans_alloc = newsize;
}
 
/* compute a(n) for any n >= 1; memoises previously computed terms */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n <= 0 */
 
    if (!tables_initialized)
        init_tables();
 
    if (ans_count < n) {
        ensure_answers_size(n);
 
        /* initialise first term if not yet done */
        if (ans_count == 0) {
            for (int m = 0; m < STATES; ++m)
                dp_prev[m] = valid_vert[m] ? (seq_t)1 : (seq_t)0;
            seq_t sum = 0;
            for (int m = 0; m < STATES; ++m)
                sum += dp_prev[m];
            answers[1] = sum;
            ans_count = 1;
        }
 
        /* subsequent terms */
        while (ans_count < n) {
            /* zero dp_next */
            for (int m = 0; m < STATES; ++m)
                dp_next[m] = (seq_t)0;
 
            /* transition */
            for (int prev = 0; prev < STATES; ++prev) {
                seq_t val = dp_prev[prev];
                if (val == (seq_t)0)
                    continue;
                for (int cur = 0; cur < STATES; ++cur) {
                    if (trans[prev][cur])
                        dp_next[cur] += val;
                }
            }
 
            /* compute sum for this width */
            seq_t sum = 0;
            for (int m = 0; m < STATES; ++m)
                sum += dp_next[m];
            answers[++ans_count] = sum;
 
            /* prepare for next iteration */
            for (int m = 0; m < STATES; ++m)
                dp_prev[m] = dp_next[m];
        }
    }
 
    return answers[n];
}
