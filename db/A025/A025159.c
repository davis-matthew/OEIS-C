/*
 * A025159  Number of partitions of n with distinct parts p(i) such that if i
 *          != j, then |p(i) - p(j)| >= 5.
 * https://oeis.org/A025159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n with distinct parts * such that
 *            |p(i)-p(j)| ≥ 5 for i ≠ j. * The function a(n) is computed by a
 *            bijection with partitions * into exactly k distinct parts: *
 *            a(n) = Σ_{k≥1} Q(n-5k(k-1)/2, k) , * where Q(m,k) counts
 *            partitions of m into k parts * (parts may repeat). Q sati...
 * Range:     u8 n <= 53, u16 n <= 143, u32 n <= 430, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _kmax_needed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_kmax_needed.h"

/* The following types are defined by the build system:
   typedef ... seq_t;  // integer type for sequence values
   typedef ... idx_t;  // integer type for indices (uint64_t)       */

static idx_t           _max_n   = 0;   /* largest n computed so far */
static int             _max_k   = 0;   /* largest k computed so far */
static seq_t*          _dp      = NULL; /* DP table flat array */
static size_t          _dp_cols = 0;   /* = _max_k + 1 */

/* --- internal helpers ----------------------------------------------------- */

static void _extend_columns(int new_kmax)
{
    size_t old_cols = _dp_cols;
    size_t new_cols = (size_t)new_kmax + 1;
    /* allocate new table */
    seq_t *newdp = (seq_t*)calloc((_max_n + 1) * new_cols, sizeof(seq_t));
    /* copy old data */
    for (idx_t w = 0; w <= _max_n; ++w) {
        for (size_t k = 0; k < old_cols; ++k) {
            newdp[w * new_cols + k] = _dp[w * old_cols + k];
        }
    }
    free(_dp);
    _dp = newdp;
    _dp_cols = new_cols;
    /* compute new columns for all existing rows */
    for (idx_t w = 1; w <= _max_n; ++w) {
        for (size_t k = old_cols; k < new_cols; ++k) {
            seq_t t1 = (w >= k) ? _dp[(w - k) * new_cols + k] : 0;
            seq_t t2 = _dp[(w - 1) * new_cols + (k - 1)];
            _dp[w * new_cols + k] = t1 + t2;
        }
    }
    _max_k = new_kmax;
}

static void _extend_rows(idx_t new_n)
{
    size_t new_rows = (size_t)new_n + 1;
    seq_t *newdp = (seq_t*)realloc(_dp, new_rows * _dp_cols * sizeof(seq_t));
    _dp = newdp;
    /* initialize new rows to zero */
    for (idx_t w = _max_n + 1; w <= new_n; ++w) {
        for (size_t k = 0; k < _dp_cols; ++k) {
            _dp[w * _dp_cols + k] = 0;
        }
    }
    /* compute DP for new rows */
    for (size_t w = _max_n + 1; w <= new_n; ++w) {
        for (size_t k = 1; k < _dp_cols && k <= w; ++k) {
            seq_t t1 = _dp[(w - k) * _dp_cols + k];
            seq_t t2 = _dp[(w - 1) * _dp_cols + (k - 1)];
            _dp[w * _dp_cols + k] = t1 + t2;
        }
    }
    _max_n = new_n;
}

static void _ensure_dp(idx_t n, int kmax)
{
    if (_dp == NULL) { /* first allocation */
        _dp_cols = (size_t)kmax + 1;
        _dp = (seq_t*)calloc((n + 1) * _dp_cols, sizeof(seq_t));
        _dp[0] = 1; /* Q(0,0)=1 */
        for (size_t w = 1; w <= n; ++w) {
            for (size_t k = 1; k < _dp_cols && k <= w; ++k) {
                seq_t t1 = _dp[(w - k) * _dp_cols + k];
                seq_t t2 = _dp[(w - 1) * _dp_cols + (k - 1)];
                _dp[w * _dp_cols + k] = t1 + t2;
            }
        }
        _max_n = n;
        _max_k = kmax;
    } else {
        if (kmax > _max_k) _extend_columns(kmax);
        if (n   > _max_n)  _extend_rows(n);
    }
}

/* --- public function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but handle n=0 gracefully */

    int kmax = _kmax_needed(n);
    _ensure_dp(n, kmax);

    seq_t result = 0;
    for (int k = 1; k <= kmax; ++k) {
        idx_t shift = (idx_t)k * (k - 1) * 5 / 2;      /* 5*k*(k-1)/2   */
        idx_t m     = n - shift;
        result += _dp[m * _dp_cols + k];
    }
    return result;
}
