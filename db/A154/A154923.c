/*
 * A154923  A symmetrical triangle sequence made from A154537:q(x,n)= Sum[(2*m
 *          + 1)^n*x^m/m!, {m, 0, Infinity}]/(Exp[x]);
 *          p(x,n)=q(x,n)+x^n*q(1/x,n); t(n,m)=coefficients(p(x,n)).
 * https://oeis.org/A154923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = coefficient of x^m in p(x, r) + coefficient of x^{r-m} in
 *            p(x, r), where r = floor((sqrt(8*n+1)-1)/2), m = n - r*(r+1)/2.
 *            p(x, r) = Q_r(x) + x^r Q_r(1/x), Q_0(x) = 1, Q_{k+1}(x) = (1 +
 *            2x) Q_k(x) + 2x Q_k'(x). Q_k is stored as coefficients of a
 *            polynomial of degree k
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t **_rows  = NULL;  /* array of rows, each row is a seq_t* */
static idx_t * _lens  = NULL;  /* length of each row   */
static idx_t  _num_rows = 0;   /* number of rows stored */

/* Ensure rows 0 .. n are computed */
static void _ensure_rows(idx_t n) {
    while (_num_rows <= n) {
        idx_t j   = _num_rows;      /* index of new row */
        idx_t len = j + 1;          /* number of coefficients */

        seq_t *row = (seq_t*)calloc(len, sizeof(seq_t));
        row[0] = 1;                 /* Q_j[0] = 1 for all j */

        if (j > 0) {
            seq_t *prev = _rows[j-1];
            for (idx_t k = 1; k < len; ++k) {
                seq_t a = 0;
                if (k <= j-1) { /* prev[k] exists */
                    a = prev[k] * (((seq_t)1) + ((seq_t)2 * k));
                }
                seq_t b = ((seq_t)2) * prev[k-1];
                row[k] = a + b;
            }
        }

        _rows = (seq_t**)realloc(_rows, (_num_rows + 1) * sizeof(seq_t*));
        _lens = (idx_t*)realloc(_lens, (_num_rows + 1) * sizeof(idx_t));
        _rows[_num_rows] = row;
        _lens[_num_rows] = len;
        _num_rows++;
    }
}

seq_t a(idx_t n) {
    /* Find the row r that contains the flattened index n. */
    idx_t k = n;
    idx_t r = 0;
    idx_t cum = 1;                    /* number of terms up to row r (inclusive) */
    while (cum <= k) {
        ++r;
        cum += r + 1;
    }
    idx_t start = cum - (r + 1);      /* first index of row r */
    idx_t m = k - start;              /* column within that row */

    _ensure_rows(r);

    seq_t val1 = _rows[r][m];
    seq_t val2 = _rows[r][r - m];
    return val1 + val2;
}
