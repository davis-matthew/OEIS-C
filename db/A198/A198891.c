/*
 * A198891  Expansion of e.g.f. 1 / [ Sum_{n>=0} (-x)^(n*(n+1)/2) /
 *          (n*(n+1)/2)! ].
 * https://oeis.org/A198891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th coefficient in the series A(x) where A(x) = 1 /
 *            Sum_{k>=0} (-x)^(k(k+1)/2) / (k(k+1)/2)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static caches for sequence values and triangular data */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static idx_t *tri_vals = NULL;
static int   *tri_sign = NULL;
static size_t tri_cap = 0;
static size_t tri_len = 0;

/* Ensure the triangular tables contain all values up to at least 'max', and that
   the inverse of 'max' is stored.  New entries are appended while expanding
   the allocated arrays as necessary. */
static void ensure_tris(idx_t max)
{
    if (tri_len == 0) {
        /* initial capacity of 16 */
        tri_cap = 16;
        tri_vals = (idx_t*)malloc(tri_cap * sizeof(idx_t));
        tri_sign = (int*)malloc(tri_cap * sizeof(int));
        tri_len = 0;
    }

    while (tri_len == 0 || tri_vals[tri_len - 1] < max) {
        idx_t j = tri_len;                      /* new index */
        idx_t val = (j * (j + 1)) / 2;          /* T_j */
        int sgn = ((j % 4 == 0) || (j % 4 == 3)) ? 1 : -1;

        /* grow the triangular array if needed */
        if (tri_len == tri_cap) {
            tri_cap *= 2;
            tri_vals  = (idx_t*)realloc(tri_vals,  tri_cap * sizeof(idx_t));
            tri_sign  = (int  *)realloc(tri_sign,  tri_cap * sizeof(int));
        }
        tri_vals[tri_len] = val;
        tri_sign[tri_len] = sgn;
        ++tri_len;
    }
}

/* Binary search in the triangular table, returns 1 if found,
   and sets '*idx' to the index j where T_j = n.  Returns 0 otherwise. */
static int triangular_index(idx_t n, size_t *idx)
{
    size_t lo = 0, hi = tri_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (tri_vals[mid] == n) {
            *idx = mid;
            return 1;
        }
        if (tri_vals[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

/* Ensure the sequence cache can store values at least up to 'n' */
static void ensure_seq_cache(idx_t n)
{
    if (seq_cache == NULL) {
        seq_cap = 1;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;  /* a(0) = 1 */
        seq_len = 1;
    }
    if (n < seq_cap)
        return;
    while (seq_cap <= n)
        seq_cap = seq_cap ? seq_cap*2 : 1;
    seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
    for (size_t i = seq_len; i < seq_cap; ++i)
        seq_cache[i] = 0;
}

/* Main function providing the sequence value. */
seq_t a(idx_t n)
{
    ensure_tris(n);
    ensure_seq_cache(n);

    if (n < seq_len)
        return seq_cache[n];

    /* Compute values from current cache size up to n */
    for (size_t k = seq_len; k <= (size_t)n; ++k) {
        /* two partial sums to avoid signed arithmetic */
        seq_t sum_pos = 0;   /* sum of terms with + sign  */
        seq_t sum_neg = 0;   /* sum of terms with - sign  */

        seq_t binom = 1;          /* C(k,0) = 1  */
        idx_t cur_t = 0;          /* current t value represented by binom */

        /* iterate over triangular numbers t <= k-1  */
        for (size_t j = 1; j < tri_len && tri_vals[j] <= (idx_t)(k - 1); ++j) {
            idx_t t_val = tri_vals[j];
            /* increase binomial coefficient to C(k, t_val) */
            for (idx_t i = cur_t; i < t_val; ++i) {
                binom = binom * (k - i) / (i + 1);
            }
            cur_t = t_val;

            seq_t term = seq_cache[k - t_val] * binom;
            if (tri_sign[j] > 0)
                sum_pos += term;
            else
                sum_neg += term;
        }

        /* r_k value (sign for k if k triangular) */
        int sign_k = 0;
        size_t j;
        if (triangular_index((idx_t)k, &j))
            sign_k = tri_sign[j];

        /* a(k) = sum_neg - sum_pos - sign_k  (sign_k in {+1,-1,0}) */
        seq_t ak = sum_neg;
        ak -= sum_pos;   /* unsigned subtraction is safe, result non‑negative */

        if (sign_k == 1)           ak -= (seq_t)1;
        else if (sign_k == -1)     ak += (seq_t)1;
        /* else sign_k == 0 : nothing to do */

        seq_cache[k] = ak;
    }

    seq_len = (size_t)n + 1;   /* at least up to n now cached */
    return seq_cache[n];
}
