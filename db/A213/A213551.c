/*
 * A213551  Rectangular array: (row n) = b**c, where b(h) = h*(h+1)/2, c(h) =
 *          b(n-1+h), n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 231, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213551 – rectangular array of convolved triangular numbers,
    read by falling antidiagonals.
    For row n we convolve b(h)=h*(h+1)/2 with c(h)=b(n-1+h) and
    then stitch the resulting array diagonally:
        a(1)=1, a(2)=6, a(3)=3, a(4)=21, a(5)=15, …            */
#include <stdlib.h>

/* static cache for the sequence values */
static seq_t *cache = NULL;      /* 0‑based array of a(k) */
static size_t cache_len = 0;     /* number of elements stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  current_diag = 0;  /* largest diagonal fully processed */

/* compute T(n,h) = sum_{j=1}^{h} b(j)*c(h-j+1)
   with b(j)=j*(j+1)/2  and  c(t)= (n-1+t)*(n+t)/2 */
static seq_t compute_T(idx_t n, idx_t h)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= h; ++j) {
        seq_t bj = (seq_t)j * ((seq_t)j + 1) / 2;
        idx_t t = n + h - j;
        seq_t cj = (seq_t)t * ((seq_t)t + 1) / 2;
        sum += bj * cj;
    }
    return sum;
}

/* ensure cache has at least arepr elements (i.e. a(repr) defined) */
static void ensure_cache(idx_t require)
{
    /* determine diagonal d so that d*(d+1)/2 >= require */
    idx_t d = current_diag;
    while ((idx_t)d * (d + 1) / 2 < require) d++;

    /* extend cache by adding terms for diags current_diag+1 .. d */
    for (idx_t diag = current_diag + 1; diag <= d; ++diag) {
        for (idx_t nrow = 1; nrow <= diag; ++nrow) {
            idx_t h = diag - nrow + 1;
            seq_t val = compute_T(nrow, h);

            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();          /* OOM */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }
    current_diag = d;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but harmless */
    if ((size_t)(n) > cache_len) {
        ensure_cache(n);
    }
    return cache[n - 1];             /* 0‑based indexing */
}
