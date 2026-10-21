/*
 * A210562  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210561; see the Formula section.
 * https://oeis.org/A210562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials v(n,x)
   Recurrence:  T(n,k) = 2*T(n-1,k-1) + T(n-2,k-1)
   with T(1,1) = 1, T(2,1) = 2, T(2,2) = 2,
   and for each n ≥ 3, the first element T(n,1) = 2. */

/* The sequence a(n) is the flattened triangle:
   a(1)=1, a(2)=2, a(3)=2, a(4)=2, a(5)=5, a(6)=4, ...   */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for the flattened triangle */
static seq_t *cache      = NULL;   /* array holding the sequence values */
static size_t cache_len  = 0;      /* number of elements currently stored */
static size_t cache_cap  = 0;      /* capacity of the cache array */

/* static buffers for the two most recent rows (as dynamic arrays) */
static seq_t *row_prev2 = NULL;    /* row n-2   (length = row_num-2) */
static seq_t *row_prev1 = NULL;    /* row n-1   (length = row_num-1) */
static idx_t  row_num   = 0;       /* the length of the row that will be computed next */

/* flag indicating whether the cache has been initialised */
static bool initialized = false;

/* utility: grow cache capacity */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* out of range, but never used */

    /* initialise data on first call */
    if (!initialized) {
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();

        /* row 1 */
        seq_t *row1 = (seq_t *)malloc(sizeof(seq_t) * 2); /* index 0 unused */
        row1[1] = 1;
        cache[cache_len++] = 1;          /* a(1) = 1 */

        /* row 2 */
        seq_t *row2 = (seq_t *)malloc(sizeof(seq_t) * 3); /* indices 1..2 */
        row2[1] = 2;
        row2[2] = 2;
        cache[cache_len++] = 2;          /* a(2) = 2 */
        cache[cache_len++] = 2;          /* a(3) = 2 */

        /* set up previous rows for recurrence */
        row_prev2 = row1;                /* row 1 */
        row_prev1 = row2;                /* row 2 */
        row_num   = 3;                   /* next row to compute (row 3) */
        initialized = true;
    }

    /* if the requested entry is already cached, return it */
    if (n <= cache_len) return cache[n - 1];

    /* otherwise build rows until we have enough entries */
    while (cache_len < (size_t)n) {
        /* allocate current row (indices 0 .. row_num) */
        seq_t *cur = (seq_t *)malloc(sizeof(seq_t) * (row_num + 1));
        if (!cur) abort();

        /* first element of every row beyond the first is 2 */
        cur[1] = 2;

        /* compute remaining elements using the recurrence */
        for (idx_t k = 2; k <= row_num; ++k) {
            seq_t a_prev1 = (k - 1 <= row_num - 1) ? row_prev1[k - 1] : 0;
            seq_t a_prev2 = (k - 1 <= row_num - 2) ? row_prev2[k - 1] : 0;
            cur[k] = (seq_t)2 * a_prev1 + a_prev2;
        }

        /* append current row to cache */
        for (idx_t k = 1; k <= row_num; ++k) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cur[k];
        }

        /* advance rows for next iteration */
        free(row_prev2);
        row_prev2 = row_prev1;
        row_prev1 = cur;
        ++row_num;
    }

    return cache[n - 1];
}
