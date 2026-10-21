/*
 * A236342  Association types in 3-dimensional algebra.
 * https://oeis.org/A236342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence for A236342 C(1) = 1, C(n) = 3⋅∑_{i1+i2=n} C(i1)C(i2)
 *            − 3⋅∑_{i1+…+i4=n} C(i1)…C(i4) + ∑_{i1+…+i8=n} C(i1)…C(i8) (n ≥
 *            2) All indices start at 1. The sums are over ordered
 *            compositions
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
/* allocate and free arrays of seq_t in C++ (constructors/destructors are run) */
static seq_t *alloc_seq_arr(size_t n) { return new seq_t[n]; }
static void free_seq_arr(seq_t *p)          { delete[] p; }
#else
/* allocate and free arrays of seq_t in C (no constructors needed) */
static seq_t *alloc_seq_arr(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
static void free_seq_arr(seq_t *p)          { free(p); }
#endif

/* static state for memoisation */
static seq_t *c     = NULL;  /* sequence a(n)          */
static seq_t *conv2 = NULL;  /* 2‑fold convolution    */
static seq_t *conv4 = NULL;  /* 4‑fold convolution    */
static seq_t *conv8 = NULL;  /* 8‑fold convolution    */
static size_t cap   = 0;     /* capacity of the arrays */
static size_t cur   = 0;     /* largest n already computed */

/* enlarge arrays so that indices up to n are available */
static void ensure_cap(idx_t n)
{
    if ((size_t)n <= cap)
        return;

    size_t newcap = cap ? cap << 1 : 128;
    while (newcap < (size_t)n)
        newcap <<= 1;

    /* allocate new arrays */
    seq_t *newc     = alloc_seq_arr(newcap + 1);
    seq_t *newconv2 = alloc_seq_arr(newcap + 1);
    seq_t *newconv4 = alloc_seq_arr(newcap + 1);
    seq_t *newconv8 = alloc_seq_arr(newcap + 1);

    /* initialise new slots to 0 */
    for (size_t i = 0; i <= newcap; ++i) {
        newc[i]     = 0;
        newconv2[i] = 0;
        newconv4[i] = 0;
        newconv8[i] = 0;
    }

    /* copy old data */
    if (c) {
        for (size_t i = 0; i <= cap; ++i) {
            newc[i]     = c[i];
            newconv2[i] = conv2[i];
            newconv4[i] = conv4[i];
            newconv8[i] = conv8[i];
        }
        free_seq_arr(c);
        free_seq_arr(conv2);
        free_seq_arr(conv4);
        free_seq_arr(conv8);
    }

    /* replace old arrays with new ones */
    c     = newc;
    conv2 = newconv2;
    conv4 = newconv4;
    conv8 = newconv8;
    cap   = newcap;
}

/* main routine: returns a(n)   */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but harmless */

    ensure_cap(n);

    if ((size_t)n <= cur)
        return c[n];              /* already computed */

    for (idx_t k = cur + 1; k <= n; ++k) {
        size_t idx = (size_t)k;

        /* 2‑fold convolution: conv2[idx] = Σ_{i=1}^{idx-1} a[i]·a[idx-i] */
        seq_t sum2 = 0;
        for (size_t i = 1; i < idx; ++i)
            sum2 += c[i] * c[idx - i];
        conv2[idx] = sum2;

        /* 4‑fold convolution: conv4[idx] = Σ_{i=1}^{idx-1} conv2[i]·conv2[idx-i] */
        seq_t sum4 = 0;
        for (size_t i = 1; i < idx; ++i)
            sum4 += conv2[i] * conv2[idx - i];
        conv4[idx] = sum4;

        /* 8‑fold convolution: conv8[idx] = Σ_{i=1}^{idx-1} conv4[i]·conv4[idx-i] */
        seq_t sum8 = 0;
        for (size_t i = 1; i < idx; ++i)
            sum8 += conv4[i] * conv4[idx - i];
        conv8[idx] = sum8;

        /* final recurrence */
        if (idx == 1) {
            c[idx] = 1;            /* a(1) = 1 */
        } else {
            seq_t val = 3 * conv2[idx];
            val      -= 3 * conv4[idx];
            val      += conv8[idx];
            c[idx] = val;
        }
    }

    cur = n;
    return c[n];
}
