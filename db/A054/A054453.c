/*
 * A054453  Triangle of partial row sums of triangle A054450(n,m), n >= m >=
 *          0.
 * https://oeis.org/A054453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = triangle of partial row sums of convolution of Fibonacci
 *            * numbers. * The function implements the explicit formula from
 *            the OEIS entry for * triangle A054450 and then flattens the
 *            triangle according to * n = r(r+1)/2 + m, 0 ≤ m ≤ r
 * Range:     u8 n <= 44, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper functions for allocating arbitrary‑precision arrays.  The
 * build supplies these headers. */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Cached values of sₙ = coefficient of Fib(x)².  These are computed
 * once and reused for all calls. */
static seq_t *s_vals = NULL;      /* array s₀ … sᵣ */
static idx_t  s_cap  = 0;         /* current allocated length   */

/* Ensure s_vals[0..limit] are known. */
static void grow_s_vals(idx_t limit)
{
    if (limit < s_cap) return;          /* nothing to do */

    idx_t new_cap = limit + 1;
    seq_t *new_s = seq_alloc_new_array(new_cap);    /* may return NULL on failure */
    if (!new_s) return;                 /* allocation failure → silently fail */

    /* Copy existing values from the old buffer.  The compiler will
     * invoke the move/copy constructor for arbitrary‑precision
     * seq_t. */
    for (idx_t i = 0; i < s_cap; ++i) new_s[i] = s_vals[i];

    /* Generate the new s values.  The initial values follow from
     * Fib(x)²: 1, 2, 5, 10.  The recurrence holds for n ≥ 4. */
    for (idx_t i = s_cap; i <= limit; ++i) {
        if (i == 0)       new_s[i] = (seq_t)1;
        else if (i == 1)  new_s[i] = (seq_t)2;
        else if (i == 2)  new_s[i] = (seq_t)5;
        else if (i == 3)  new_s[i] = (seq_t)10;
        else
            new_s[i] = 2 * new_s[i-1] + new_s[i-2]
                      - 2 * new_s[i-3] - new_s[i-4];
    }

    /* Free the old buffer and update pointers/sizes. */
    if (s_vals) seq_free_array(s_vals);
    s_vals = new_s;
    s_cap  = new_cap;
}

/* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.  This works
 * for 64‑bit indices and uses 128‑bit intermediate to avoid overflow. */
static idx_t row_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Compute a(r,m) for r ≥ m ≥ 0 using the explicit formulas derived
 * from the generating function. */
static seq_t a_of_row_col(idx_t r, idx_t m)
{
    grow_s_vals(r);                        /* make sure s_0 … s_r exist */

    if (m == 0) {                          /* first column: simple difference */
        seq_t val = s_vals[r];
        if (r >= 2) val -= s_vals[r-2];
        return val;
    }

    /* Columns m ≥ 1:
     * Sum_{j=0}^{⌊(r-m)/2⌋} C(m-2+j, j) * s_{r-m-2j}. */
    seq_t sum   = 0;
    idx_t max_j = (r - m) >> 1;
    seq_t binom = 1;                        /* C(m-2,0) */

    for (idx_t j = 0; j <= max_j; ++j) {
        idx_t idx = r - m - (j << 1);       /* r-m-2j */
        sum += binom * s_vals[idx];
        if (j == max_j) break;
        /* Next binomial coefficient:
         * C(m-2+j+1, j+1) = C(m-2+j, j) * (m-2+j+1)/(j+1). */
        binom = binom * (seq_t)(m - 2 + j + 1) / (seq_t)(j + 1);
    }
    return sum;
}

/* -----------------------------------------------------------------
 *  Public entry required by the framework.
 *  The function returns the n‑th term of A054453 (flattened triangle).
 * ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);                                /* row index */
    idx_t start = (r * (r + 1)) >> 1;                   /* first index in this row */
    idx_t m = n - start;                                /* column index */

    return a_of_row_col(r, m);
}
