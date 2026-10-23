/*
 * A126265  Triangle of coefficients of q in e.g.f. that satisfies: A(x,q) =
 *          exp( q*x*A(q*x,q) ), read by rows of [n*(n-1)/2 + 1] terms in row
 *          n for n>=0.
 * https://oeis.org/A126265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 68, u32 n <= 242, u64 n <= 861,
 *            u128 n <= 3748, big any n
 * Helpers:   _new_row, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126265 – triangle of coefficients of q in the e.g.f.
   A(x,q) = exp( q*x*A(q*x,q) )
   a(n) is obtained by reading the triangle row by row.
   The recurrence for the polynomial f_n(q) is
        f_0(q) = 1
        f_n(q) = Σ_{k=1}^{n} binom(n‑1,k‑1) * k * q^{k‑1} *
                 f_{k‑1}(q) * f_{n‑k}(q)      (n ≥ 1)
   where f_n(q) = Σ_{j=0}^{n*(n‑1)/2} T(n,j) q^j.
   We compute the rows incrementally, caching each row in a dynamic
   array.  The algorithm runs in O(r³) for the r‑th row, which is
   easily fast enough for n ≤ 10000.  All arithmetic is performed in
   seq_t and indices in idx_t.  The binomial coefficients are obtained
   from the provided utility binom().
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/_new_row.h"

static idx_t    _row_cnt   = 0;          /* number of rows computed          */
static idx_t    _total_entries = 0;     /* total number of elements stored   */
static idx_t    _row_capacity = 0;      /* capacity of the row metadata      */

static seq_t  **_polys    = NULL;       /* pointers to each row              */
static size_t  *_row_len  = NULL;       /* length of each row                */
static idx_t   *_row_start= NULL;       /* starting index of each row        */

/* Grow the row metadata arrays to at least min_cap entries.
   The arrays are reallocated with appropriate casts for C++ and C. */
static void _grow_row_meta(idx_t min_cap)
{
    if (_row_capacity >= min_cap)
        return;

    idx_t new_cap = (_row_capacity == 0) ? 4 : _row_capacity;
    while (new_cap < min_cap)
        new_cap <<= 1;

    size_t *nl = (size_t *)realloc(_row_len, new_cap * sizeof(size_t));
    if (!nl) abort();
    _row_len = nl;

    idx_t *rs = (idx_t *)realloc(_row_start, new_cap * sizeof(idx_t));
    if (!rs) abort();
    _row_start = rs;

    seq_t **np = (seq_t **)realloc(_polys, new_cap * sizeof(seq_t *));
    if (!np) abort();
    _polys = np;

    _row_capacity = new_cap;
}

/* Ensure that all rows up to (and including) the one containing
   index n are computed and cached. */
static void _ensure_upto(idx_t n)
{
    while (_total_entries <= n) {
        idx_t r = _row_cnt;                      /* current row index   */
        size_t len = (size_t)r * ((size_t)r - 1) / 2 + 1;

        _grow_row_meta(_row_cnt + 1);

        seq_t *row = _new_row(len);
        _row_start[_row_cnt] = _total_entries;
        _row_len[_row_cnt] = len;
        _polys[_row_cnt] = row;

        if (r == 0) {
            row[0] = 1;          /* f_0 = 1 */
        } else {
            /* compute f_r using the recurrence */
            for (idx_t k = 1; k <= r; ++k) {
                seq_t *a = _polys[k - 1];
                size_t len_a = _row_len[k - 1];
                seq_t *b = _polys[r - k];
                size_t len_b = _row_len[r - k];

                seq_t coeff = binom(r - 1, k - 1);
                coeff = coeff * (seq_t)k;   /* coefficient in recurrence */

                idx_t shift = k - 1;        /* q^{k-1} shift */

                for (idx_t i = 0; i < len_a; ++i) {
                    seq_t a_val = a[i];
                    for (idx_t j = 0; j < len_b; ++j) {
                        seq_t prod = coeff * a_val * b[j];
                        idx_t idx = i + j + shift;
                        row[idx] += prod;
                    }
                }
            }
        }

        _total_entries += len;
        ++_row_cnt;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    _ensure_upto(n);

    /* locate the row that contains the desired index */
    for (idx_t r = 0; r < _row_cnt; ++r) {
        idx_t start = _row_start[r];
        idx_t len   = _row_len[r];
        if (n < start + len)
            return _polys[r][n - start];
    }
    /* should never happen */
    return 0;
}
