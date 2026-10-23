/*
 * A133404  Table of sum of numerator and denominator of Farey sequences, read
 *          by rows.
 * https://oeis.org/A133404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = sum of numerator and denominator of nth fraction * when
 *            all fractions of Farey sequences of increasing order are listed
 *            * row by row, starting with 0/1. The first term a(1) corresponds
 *            to * 0/1, the second to 1/1, the third to 0/1 of the next row,
 *            etc
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*----------------------------------------------------------------------------*/
/*  Global static tables for phi and cumulative term counts up to order m   */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>

/* Number of terms already precomputed (maximum order m) */
static idx_t _max_order = 0;
/* Euler's totient table phi[0..m] (we ignore phi[0]) */
static idx_t *_phi = NULL;
/* Cumulated number of fractions up to order i (cum[i] = Σ_{k=1..i}(1+Σ_{j=1..k}φ(j)) ) */
static seq_t *_cum = NULL;

/* Recompute phi[0..m] and cumulative counts up to order m.  New data replaces
 * the old data in the global tables.  This is called only when we need a larger
 * order than currently stored. */
static void _ensure_up_to(idx_t m)
{
    if (m <= _max_order) return;          /* already have enough data */

    /* Allocate larger arrays */
    _phi  = (idx_t *)realloc(_phi,  (m + 1) * sizeof(idx_t));
    _cum  = (seq_t *)realloc(_cum,  (m + 1) * sizeof(seq_t));
    if (!_phi || !_cum) return;            /* allocation failed – nothing we can do */

    /* Compute Euler’s totient function for 1..m using a classic sieve */
    for (idx_t i = 0; i <= m; ++i) _phi[i] = i;
    for (idx_t i = 2; i <= m; ++i)
        if (_phi[i] == i)                /* i is prime */
            for (idx_t j = i; j <= m; j += i)
                _phi[j] -= _phi[j] / i;

    /* Build cumulative counts */
    seq_t sum_phi = 0;
    seq_t cum     = 0;
    _cum[0] = 0;                       /* zero order has zero terms */
    for (idx_t i = 1; i <= m; ++i) {
        sum_phi += (seq_t)_phi[i];
        cum += (seq_t)1 + sum_phi;      /* 1 (for 0/1) + sums of φ up to i */
        _cum[i] = cum;
    }

    _max_order = m;
}

/*----------------------------------------------------------------------------*/
/*  Main function required by the problem                                */
/*----------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1 guarantees n ≥ 1 in practice */

    /* Ensure we have enough precomputed data for this n.  Increase m
       geometrically until the cumulative count exceeds n. */
    if (_cum == NULL) _ensure_up_to(1);
    while (_cum[_max_order] < n)        /* not enough – double the bound */
        _ensure_up_to(_max_order * 2 + 1);

    /* Binary search for the smallest m with cum[m] ≥ n */
    idx_t low = 1, high = _max_order;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (_cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t order = low;                   /* order m of the desired row */

    /* Position of the fraction within this row (0‑based, 0→0/1) */
    idx_t pos = (idx_t)(n - _cum[order - 1] - 1);

    if (pos == 0)                        /* special case: 0/1 */
        return (seq_t)1;

    /* Generate the Farey sequence of the given order until we reach the
       required fraction.  The algorithm used is the standard iterative
       method starting with 0/1 and 1/order. */
    idx_t a_num = 0, a_den = 1;          /* left fraction a/b   */
    idx_t c_num = 1, c_den = order;      /* right fraction c/d  */

    for (idx_t step = 0; step < pos - 1; ++step) {   /* advance (pos-1) times */
        idx_t k = (order + a_den) / c_den;
        idx_t e = k * c_num - a_num;
        idx_t f = k * c_den - a_den;
        a_num = c_num;  a_den = c_den;
        c_num = e;      c_den = f;
    }

    return (seq_t)(c_num + c_den);
}
