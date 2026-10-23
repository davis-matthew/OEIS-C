/*
 * A010329  Expressible as C(m,3) + C(n,3) in at least 3 ways.
 * https://oeis.org/A010329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010329: nth smallest integer that can be written as
   C(m,3)+C(n,3) in at least three different unordered ways. */
#include <stdlib.h>

static seq_t * _fvals = NULL;
static seq_t * _seq   = NULL;
static size_t  _seq_len = 0;
static size_t  _seq_cap = 0;
static idx_t   _M = 0;

/* comparison for qsort */
static int seq_cmp(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* rebuild the sequence up to at least n terms */
static void rebuild(idx_t n) {
    while (_seq_len < n) {
        if (_M == 0) _M = 2000;          /* initial size */
        /* free previous buffers */
        free(_fvals);
        free(_seq);

        /* generate f(m)=C(m,3) for m = 3 .. 3+M-1 */
        size_t tot_pairs = (_M * (_M + 1)) / 2;
        _fvals = (seq_t *)malloc(_M * sizeof(seq_t));
        for (idx_t i = 0; i < _M; ++i) {
            idx_t m = i + 3;                     /* m >= 3 */
            _fvals[i] = (seq_t)m * (seq_t)(m-1) * (seq_t)(m-2) / 6;
        }

        /* generate all pairwise sums */
        seq_t *sums = (seq_t *)malloc(tot_pairs * sizeof(seq_t));
        size_t pos = 0;
        for (idx_t i = 0; i < _M; ++i) {
            for (idx_t j = i; j < _M; ++j) {
                sums[pos++] = _fvals[i] + _fvals[j];
            }
        }

        /* sort the sums */
        qsort(sums, tot_pairs, sizeof(seq_t), seq_cmp);

        /* collect values that appear at least 3 times */
        _seq_cap = 16;
        _seq = (seq_t *)malloc(_seq_cap * sizeof(seq_t));
        /* we rebuild seq each time */
        _seq_len = 0;
        for (size_t i = 0; i < tot_pairs; ) {
            size_t j = i + 1;
            while (j < tot_pairs && sums[j] == sums[i]) ++j;
            size_t cnt = j - i;
            if (cnt >= 3) {
                if (_seq_len == _seq_cap) {
                    _seq_cap <<= 1;
                    _seq = (seq_t *)realloc(_seq, _seq_cap * sizeof(seq_t));
                }
                _seq[_seq_len++] = sums[i];
            }
            i = j;
        }

        free(sums);
        free(_fvals);

        /* double M for next round if needed */
        if (_seq_len < n) _M <<= 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (_seq_len < n) rebuild(n);
    return _seq[n-1];
}
