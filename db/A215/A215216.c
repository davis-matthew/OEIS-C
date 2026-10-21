/*
 * A215216  Coefficient triangle of the Hermite-Bell polynomials for power -2.
 * https://oeis.org/A215216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(0)=1, a(1)=2. Coefficients c_{r,k} of the Hermite‑Bell
 *            polynomials for power –2 satisfy c_{0,0}=1 c_{r+1,0}=2*c_{r,0}
 *            for k>=1: c_{r+1,k}=2*c_{r,k} + (2*(k-1)-3*r)*c_{r,k-1} The
 *            sequence a(n) is the flattened list of all c_{r,k} taken
 *            row‑wise (row 0 has length 1, row r>=1 has length r)
 * Range:     i8 n <= 7, i16 n <= 23, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* flattened coefficients */
static size_t cache_len   = 0;    /* number of terms stored       */
static size_t cache_cap   = 0;    /* capacity of cache            */

static seq_t *prev_coeff = NULL; /* coefficients of current last row */
static size_t prev_len   = 0;     /* length of prev_coeff            */
static idx_t  current_row = 0;   /* index of row current in prev_coeff */

/* enlarge cache to hold at least 'needed' elements */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* initialise on first call */
static void init_sequence(void)
{
    if (cache) return;

    cache_cap = 256;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

    /* first term a(0) = 1 */
    cache[0] = 1;
    cache_len = 1;

    /* initialise previous row (row 0) */
    prev_len = 1;
    prev_coeff = (seq_t *)malloc(sizeof(seq_t));
    prev_coeff[0] = 1;
    current_row = 0;
}

/* compute a(n) for arbitrary n */
seq_t a(idx_t n)
{
    init_sequence();

    /* ensure we have enough space for all terms up to n */
    ensure_cache((size_t)n + 1);

    /* grow triangular tables until we have at least (n+1) values */
    while (cache_len <= (size_t)n) {
        /* compute next row */
        idx_t next_row   = current_row + 1;
        size_t curr_len  = (size_t)next_row;          /* r+1 terms */

        seq_t *curr_coeff = (seq_t *)malloc(curr_len * sizeof(seq_t));

        /* k = 0 */
        curr_coeff[0] = (seq_t)2 * prev_coeff[0];

        /* k >= 1 */
        for (size_t k = 1; k < curr_len; ++k) {
            seq_t term1 = (k < prev_len) ? (seq_t)2 * prev_coeff[k] : 0;
            seq_t coeff = (seq_t)(2 * (k - 1)) - (seq_t)3 * (seq_t)current_row;
            seq_t term2 = coeff * prev_coeff[k - 1];
            curr_coeff[k] = term1 + term2;
        }

        /* append current row to cache */
        ensure_cache(cache_len + curr_len);
        for (size_t k = 0; k < curr_len; ++k)
            cache[cache_len++] = curr_coeff[k];

        /* advance to next row */
        free(prev_coeff);
        prev_coeff = curr_coeff;
        prev_len = curr_len;
        current_row = next_row;
    }

    return cache[(size_t)n];
}
