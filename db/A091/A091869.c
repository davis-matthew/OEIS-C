/*
 * A091869  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n having k peaks at even height.
 * https://oeis.org/A091869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 215, u64 n <= 711,
 *            u128 n <= 2500, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = flattened triangle entry T(r,k) where 
 * T(n,k) = M(n-k-1) * binomial(n-1,k)
 * and M(m) are Motzkin numbers (A001006). The index n starts at 1.
 * The triangle is read row‑by‑row: T(1,0), T(2,0), T(2,1), T(3,0), ...
 */

#include <stddef.h>   /* size_t */
#include <math.h>     /* sqrtl, etc. */
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */

static seq_t *motzkin = NULL;      /* cache of Motzkin numbers M[0..] */
static size_t motzkin_cap = 0;     /* cache capacity */

/* Expand Motzkin cache to at least 'needed' size (0‑based). */
static void
ensure_motzkin(size_t needed)
{
    if (needed < motzkin_cap)
        return;
    size_t new_cap = motzkin_cap == 0 ? 4 : motzkin_cap * 2;
    if (new_cap <= needed) {
        new_cap = needed + 4;
    }
    motzkin = (seq_t *)realloc(motzkin, new_cap * sizeof(seq_t));
    /* initialize new entries to 0 before computing */
    for (size_t i = motzkin_cap; i < new_cap; ++i) {
        motzkin[i] = (seq_t)0;
    }
    /* compute Motzkin numbers up to 'new_cap - 1' */
    for (size_t i = motzkin_cap; i < new_cap; ++i) {
        if (i == 0) {
            motzkin[i] = (seq_t)1;
        } else if (i == 1) {
            motzkin[i] = (seq_t)1;
        } else {
            /* M(i) = ((2*i+1)*M(i-1)+(3*i-3)*M(i-2))/(i+2) */
            seq_t term1 = motzkin[i - 1] * (seq_t)(2 * i + 1);
            seq_t term2 = motzkin[i - 2] * (seq_t)(3 * i - 3);
            seq_t num   = term1 + term2;
            seq_t den   = (seq_t)(i + 2);
            motzkin[i]  = num / den;
        }
    }
    motzkin_cap = new_cap;
}

/* Main routine requested by the framework. */
seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 1. */
    if (n < 1)
        return (seq_t)0;

    /* Determine the row r such that r*(r+1)/2 >= n. */
    idx_t r;
    /* Initial estimate using quadratic formula (double). */
    long double est = (sqrtl((long double)8 * (long double)n + 1.0L) - 1.0L) / 2.0L;
    r = (idx_t)est;
    if (r < 1)
        r = 1;

    /* Adjust using integer arithmetic to guarantee r*(r+1)/2 >= n. */
    while ( ( (__int128)r * (r + 1) ) / 2 < n )
        ++r;
    while ( (r > 1) && ( (__int128)(r - 1) * r ) / 2 >= n )
        --r;

    /* Index within the row: k = offset - 1, offset = n - prev_row_tri + 1. */
    idx_t prev_tri = ( (idx_t)(((idx_t)(r - 1) * r) / 2) );  /* (r-1)*r/2 */
    idx_t offset   = n - prev_tri;          /* 1‑based offset in the row */
    idx_t k        = offset - 1;            /* 0‑based column index */

    /* Compute T(r,k) = M(r-k-1) * binomial(r-1,k). */
    idx_t m = (r - 1) - k;                  /* argument for Motzkin number */

    ensure_motzkin(m + 1);                  /* make sure M[m] exists */

    seq_t bin = binomial(r - 1, k);         /* binomial coefficient */
    seq_t motz = motzkin[m];                /* Motzkin number */

    return bin * motz;
}
