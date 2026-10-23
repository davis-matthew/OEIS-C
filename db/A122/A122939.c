/*
 * A122939  G.f. A(x) satisfies: A(x+x^2) = A(x)^2/(1+x)^2.
 * https://oeis.org/A122939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies A(x+x^2)=A(x)^2/(1+x)^2, a(0)=1. Recurrence
 *            derived from coefficient comparison: a(n) = L(n) - S1(n) - T(n)
 *            where L(n) = Σ_{j=ceil(n/2)}^{n-1} a[j] * C(j, n-j) S1(n) =
 *            Σ_{i=1}^{n-1} a[i] * a[n-i] T(n) = Σ_{k=0}^{n-1} d[k] *
 *            (−1)^{n-k} * (n-k+1) d[k] = Σ_{i=0}^{k} a[i] * a[k-i] (= 2...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- utility (binomial) --------------------------------- */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* ---------- memoisation storage -------------------------------- */
static idx_t _computed = 0;      /* number of terms already computed (last index+1) */
static idx_t _cap = 0;           /* size of allocated arrays */
static seq_t * _a_vals = NULL;   /* a[0 .. _computed-1] */
static seq_t * _d_vals = NULL;   /* d[0 .. _computed-1] */

/* ensure capacity for at least 'needed' elements (needed > 0)   */
static void _ensure_capacity(idx_t needed)
{
    if (needed <= _cap) return;
    size_t new_cap = (size_t)needed;
    seq_t * new_a = (seq_t *)realloc(_a_vals, new_cap * sizeof(seq_t));
    seq_t * new_d = (seq_t *)realloc(_d_vals, new_cap * sizeof(seq_t));
    /* initialise the newly allocated cells to 0 */
    for (size_t i = _cap; i < new_cap; ++i) {
        new_a[i] = 0;
        new_d[i] = 0;
    }
    _a_vals = new_a;
    _d_vals = new_d;
    _cap = new_cap;
}

/* ---------- main sequence function ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* initialise storage and base term if first call            */
    if (_computed == 0) {
        _ensure_capacity(1);
        _a_vals[0] = (seq_t)1;
        _d_vals[0] = (seq_t)1;
        _computed  = 1;
    }

    /* storage must accommodate index n                      */
    _ensure_capacity(n + 1);

    /* compute missing terms up to n                           */
    for (idx_t m = _computed; m <= n; ++m) {   /* m is new index to compute */
        /* --------- L(m) ------------------------------------- */
        seq_t L = 0;
        idx_t start = (m + 1) / 2;           /* ceil(m/2) */
        for (idx_t j = start; j < m; ++j) {
            idx_t r = m - j;                 /* r = m-j, 1 <= r <= j          */
            seq_t bin = _binom(j, r);
            L += _a_vals[j] * bin;
        }

        /* --------- S1(m) ----------------------------------- */
        seq_t S1 = 0;
        for (idx_t i = 1; i < m; ++i) {
            S1 += _a_vals[i] * _a_vals[m - i];
        }

        /* --------- T(m) ------------------------------------ */
        seq_t T_pos = 0, T_neg = 0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t r = m - k;
            seq_t mult = _d_vals[k] * (seq_t)(r + 1);
            if ((r & 1) == 0)   /* even exponent => + sign */
                T_pos += mult;
            else
                T_neg += mult;
        }

        /* --------- determine a(m) -------------------------- */
        __int128 tmp = (__int128)L - (__int128)S1 - (__int128)T_pos + (__int128)T_neg;
        seq_t a_m = (seq_t)tmp;                /* result fits into seq_t by construction */
        _a_vals[m] = a_m;

        /* --------- compute d(m) = Σ a(i)*a(m-i) ------------ */
        _d_vals[m] = a_m + a_m + S1;           /* 2*a_m + S1(m) */
    }

    _computed = n + 1;
    return _a_vals[n];
}
