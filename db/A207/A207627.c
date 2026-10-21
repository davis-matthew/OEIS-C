/*
 * A207627  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207628; see the Formula section.
 * https://oeis.org/A207627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element of the flattened triangle defined by
 *            the recurrences u(1,x)=1, v(1,x)=1 u(n,x)=u(n-1,x)+v(n-1,x)
 *            v(n,x)=2x*u(n-1,x)+2x*v(n-1,x)+1 Each row n contains first the
 *            coefficient of x^(n-2) in u(n,x) and then the coefficients of
 *            u(n+1,x) from degree 0 to n-1
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state */
static bool initialized = false;
static seq_t *cache   = NULL;
static size_t cache_len = 0, cache_cap = 0;

static seq_t *u_prev = NULL, *v_prev = NULL;
static size_t u_prev_len = 0, v_prev_len = 0;
static idx_t cur_row = 0;          /* current row index whose u_prev is u(row,x) */

/* make sure cache can hold `needed` elements */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* initialise static buffers and produce a(1)=1 */
static void init(void)
{
    /* cache for the sequence */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)1;
    cache_len = 1;

    /* u(1,x) = 1 */
    u_prev_len = 1;
    u_prev = (seq_t *)malloc(u_prev_len * sizeof(seq_t));
    u_prev[0] = (seq_t)1;

    /* v(1,x) = 1 */
    v_prev_len = 1;
    v_prev = (seq_t *)malloc(v_prev_len * sizeof(seq_t));
    v_prev[0] = (seq_t)1;

    /* compute u(2) and v(2) */
    size_t len_u_next = u_prev_len;           /* 1 */
    seq_t *u_next = (seq_t *)calloc(len_u_next, sizeof(seq_t));
    u_next[0] = u_prev[0] + v_prev[0];

    size_t len_v_next = u_prev_len + 1;       /* 2 */
    seq_t *v_next = (seq_t *)calloc(len_v_next, sizeof(seq_t));
    /* 2x*u(1) and 2x*v(1) */
    v_next[1] += (seq_t)2 * u_prev[0];
    v_next[1] += (seq_t)2 * v_prev[0];
    /* constant term +1 */
    v_next[0] = (seq_t)1;

    free(u_prev);
    free(v_prev);
    u_prev = u_next;
    v_prev = v_next;
    u_prev_len = len_u_next;
    v_prev_len = len_v_next;
    cur_row = 2;        /* next row to produce will be row 2 */
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    while (cache_len < (size_t)n) {
        /* ensure cache capacity for the whole new row */
        ensure_cache_size(cache_len + cur_row);

        /* first element of row: coefficient of x^(cur_row-2) in u_prev */
        cache[cache_len++] = u_prev[u_prev_len - 1];

        /* compute coefficients for the next row (u_next, v_next) */
        size_t len_u_next = v_prev_len;           /* length of u(row+1) = v(row) */
        seq_t *u_next = (seq_t *)calloc(len_u_next, sizeof(seq_t));
        for (size_t i = 0; i < v_prev_len; ++i) {
            seq_t val_u = (i < u_prev_len) ? u_prev[i] : (seq_t)0;
            u_next[i] = val_u + v_prev[i];
        }

        size_t len_v_next = v_prev_len + 1;       /* length of v(row+1) = v(row)+1 */
        seq_t *v_next = (seq_t *)calloc(len_v_next, sizeof(seq_t));
        v_next[0] = (seq_t)1;                     /* constant term */

        for (size_t i = 0; i < u_prev_len; ++i)
            v_next[i + 1] += (seq_t)2 * u_prev[i];
        for (size_t i = 0; i < v_prev_len; ++i)
            v_next[i + 1] += (seq_t)2 * v_prev[i];

        /* remaining elements of the row: coefficients of u_next[0 .. cur_row-2] */
        for (size_t j = 1; j < cur_row; ++j)
            cache[cache_len++] = u_next[j - 1];

        /* advance to next row, freeing old polynomials */
        free(u_prev);
        free(v_prev);
        u_prev = u_next;
        v_prev = v_next;
        u_prev_len = len_u_next;
        v_prev_len = len_v_next;
        ++cur_row;
    }

    return cache[(size_t)n - 1];
}
