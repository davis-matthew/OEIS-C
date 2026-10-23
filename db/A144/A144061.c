/*
 * A144061  Eigentriangle generated from A109128, row sums = expansion of
 *          {2(exp(x)-1)}.
 * https://oeis.org/A144061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*binomial(r-1,k-1)-1) * A001861(k-1) where r is the row
 *            (1‑based) such that r(r+1)/2 ≥ n, and k = n - r(r-1)/2 is the
 *            column (1‑based) within that row
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial_n_k.h"

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Compute the A001861 numbers needed for the triangle.  They are
    defined recursively:
    a(0) = 1
    a(m) = Σ_{k=1..m} (2*binomial(m-1,k-1)-1)*a(k-1)
    The table is grown on demand. */
static seq_t* ap = NULL;        /* cache for A001861 values */
static size_t ap_cap = 0;       /* capacity of the array          */
static idx_t  ap_used = 0;      /* number of values stored (≥1)  */

static void ensure_ap(idx_t need) /* need = j+1 for j = k-1 */
{
    if (ap_used >= need)
        return;

    if (ap_used == 0) {               /* first allocation */
        ap_cap = 8;
        ap = (seq_t*)malloc(ap_cap * sizeof(seq_t));
        if (!ap) return;              /* out of memory – caller will get 0 */
        ap[0] = (seq_t)1;
        ap_used = 1;
    }

    /* grow if necessary */
    if ((size_t)need > ap_cap) {
        size_t new_cap = ap_cap ? ap_cap : 8;
        while (new_cap < (size_t)need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(ap, new_cap * sizeof(seq_t));
        if (!tmp) return;             /* out of memory */
        ap = tmp;
        ap_cap = new_cap;
    }

    /* compute missing entries */
    for (idx_t m = ap_used; m < need; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;          /* binomial(m-1,0) */
        for (idx_t t = 1; t <= m; ++t) {
            seq_t coeff = (seq_t)2 * bin - (seq_t)1;
            sum += coeff * ap[t - 1];
            if (t < m)
                bin = bin * (seq_t)(m - t) / (seq_t)t; /* next binomial */
        }
        ap[m] = sum;
    }

    ap_used = need;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n = 0 */

    /* Find row r (1‑based) such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    idx_t tri = 1;              /* current sum of row lengths (i.e. r(r+1)/2) */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* Column k (1‑based) within that row. */
    idx_t k = n - tri + r;
    idx_t j = k - 1;            /* zero‑based index for A001861 */

    /* Ensure A001861 table contains value up to j. */
    ensure_ap(j + 1);
    if (ap_used == 0)  /* memory failure */
        return (seq_t)0;

    /* Compute binomial(r-1, k-1). */
    seq_t bin = (seq_t)1;
    idx_t N = r - 1;
    idx_t rk = k - 1;
    for (idx_t i = 1; i <= rk; ++i) {
        bin = bin * (seq_t)(N - i + 1) / (seq_t)i;
    }

    seq_t coef = (seq_t)2 * bin - (seq_t)1;
    seq_t result = coef * ap[j];
    return result;
}
