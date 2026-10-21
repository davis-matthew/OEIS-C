/*
 * A086662  Stirling transform of Catalan numbers: Sum_{k=0..n} |Stirling1(n,
 *          k)|*binomial(2*k, k)/(k+1).
 * https://oeis.org/A086662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} |Stirling1(n,k)| * Catalan(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (Catalan(k) = binomial(2k,k)/(k+1) ) */

#include <stdlib.h>
#if defined(__cplusplus)
#  include <new>
#endif

static seq_t *a_vals   = NULL;   /* a(n) values                    */
static seq_t *cat_vals = NULL;   /* Catalan numbers Cat(k)         */
static seq_t *prev_row = NULL;   /* previous row of |Stirling1|    */
static seq_t *curr_row = NULL;   /* current  row of |Stirling1|    */
static idx_t  max_n    = (idx_t)-1;  /* largest n computed so far     */

/* Helper: construct newly allocated seq_t elements (C++ only) */
#if defined(__cplusplus)
static inline void construct_seq_at(seq_t *arr, idx_t i)
{
    new (&arr[i]) seq_t();
}
#endif

/* Ensure that all internal tables contain data up to index n. */
static void ensure_cache(idx_t n)
{
    /* First allocation (for n = 0) */
    if (a_vals == NULL) {
        size_t sz = (size_t)1;
        a_vals   = (seq_t*)malloc(sz * sizeof(seq_t));
        cat_vals = (seq_t*)malloc(sz * sizeof(seq_t));
        prev_row = (seq_t*)malloc(sz * sizeof(seq_t));
        curr_row = (seq_t*)malloc(sz * sizeof(seq_t));

#if defined(__cplusplus)
        construct_seq_at(a_vals,   0);
        construct_seq_at(cat_vals, 0);
        construct_seq_at(prev_row, 0);
        construct_seq_at(curr_row, 0);
#endif

        cat_vals[0] = (seq_t)1;          /* Cat(0) = 1 */
        prev_row[0] = (seq_t)1;          /* S(0,0) = 1 */
        a_vals[0]   = (seq_t)1;          /* a(0)   = 1 */
        max_n = 0;
    }

    /* Nothing to do if we've already covered n */
    if (n <= max_n) return;

    /* Grow all arrays to size n+1. */
    size_t old_size = (size_t)(max_n + 1);
    size_t new_size = (size_t)(n + 1);

    a_vals   = (seq_t*)realloc(a_vals,   new_size * sizeof(seq_t));
    cat_vals = (seq_t*)realloc(cat_vals, new_size * sizeof(seq_t));
    prev_row = (seq_t*)realloc(prev_row, new_size * sizeof(seq_t));
    curr_row = (seq_t*)realloc(curr_row, new_size * sizeof(seq_t));

#if defined(__cplusplus)
    for (idx_t i = old_size; i <= n; ++i) {
        construct_seq_at(a_vals,   i);
        construct_seq_at(cat_vals, i);
        construct_seq_at(prev_row, i);
        construct_seq_at(curr_row, i);
    }
#endif

    /* Compute rows successively from max_n+1 up to n. */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* Catalan number Cat(m) = Cat(m-1) * (4*m-2)/(m+1). */
        seq_t prev_cat = cat_vals[m - 1];
        seq_t num = prev_cat * (seq_t)(4 * m - 2);   /* 4*m-2 == 4*(m-1)+2 */
        seq_t denom = (seq_t)(m + 1);
        cat_vals[m] = num / denom;

        /* Compute the m‑th row of unsigned Stirling numbers (first kind). */
        curr_row[0] = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t val = prev_row[k - 1];
            if (k <= m - 1) {
                val += (seq_t)(m - 1) * prev_row[k];
            }
            curr_row[k] = val;
        }

        /* Accumulate a(m) = Σ_k curr_row[k] * Cat(k). */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k) {
            sum += curr_row[k] * cat_vals[k];
        }
        a_vals[m] = sum;

        /* Swap rows for next iteration. */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
    }

    max_n = n;
}

/* External linkage as requested. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_vals[n];
}
