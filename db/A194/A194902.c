/*
 * A194902  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=-sqrt(12).
 * https://oeis.org/A194902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) computes the A194902 sequence entry by extracting the
 *            appropriate element from the row permutation based on the
 *            ordering of fractional parts {−√12·k}
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned values */
#include <stdlib.h>
#include <stdint.h>

/* Return floor(sqrt(x)) for x up to 2^64-1 */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* start with the highest power of two less than or equal to sqrt(max_uint64) */
    idx_t bit = 1ULL << 32;          /* 2^32 < sqrt(2^64-1) < 2^32+1 */
    while (bit) {
        idx_t next = r + bit;
        if (next <= x / next)        /* avoid overflow */
            r = next;
        bit >>= 1;
    }
    return r;
}

/* Global pointer used by the comparator to access the s_k values */
static const idx_t *g_sarray = NULL;

/* Comparator for sorting indices 1..r in descending order of fractional parts
   of k*√12 (i.e., ascending order of fractional parts of −√12·k). */
static int cmp_indices(const void *pa, const void *pb)
{
    idx_t a = *(const idx_t*)pa;
    idx_t b = *(const idx_t*)pb;
    if (a == b) return 0;

    idx_t d = (a < b) ? (b - a) : (a - b);
    idx_t diff_s = (a < b) ? (g_sarray[b-1] - g_sarray[a-1])
                           : (g_sarray[a-1] - g_sarray[b-1]);

    unsigned long long left  = 12ULL * d * d;
    unsigned long long right = (unsigned long long)diff_s * diff_s;

    /* For descending order of fractional parts: a before b if f_a > f_b,
       which occurs when 12*d^2 < diff_s^2. */
    if (left < right)  return -1;   /* a before b */
    if (left > right)  return  1;   /* a after  b */
    return 0;                      /* equal (unlikely for irrational α) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    /* Find the row r such that T(r-1) < n <= T(r), T(r)=r*(r+1)/2. */
    idx_t disc = 1 + 8 * n;                /* 1 + 8n fits in 64 bits */
    idx_t sqrt_disc = isqrt_u64(disc);
    idx_t r = (sqrt_disc - 1) / 2;         /* floor of the quadratic solution */

    /* Correct possible off‑by‑one due to integer sqrt */
    while ((r * (r + 1)) / 2 < n) r++;
    while (r > 0 && ((r - 1) * r) / 2 >= n) r--;

    idx_t prev = ((r - 1) * r) / 2;        /* elements before the current row */
    idx_t pos  = n - prev;                 /* 1‑based position within the row */

    if (r == 0) return 0;                 /* safety check, should not happen */

    /* Allocate array of floor_k = floor(sqrt(12*k^2)) for k=1..r */
    idx_t *sarray = (idx_t*)malloc(r * sizeof(idx_t));
    if (!sarray) return 0;                /* allocation failure */

    for (idx_t k = 1; k <= r; ++k) {
        idx_t val = 12 * k * k;            /* 12*k^2 fits in 64 bits for k<=10000 */
        sarray[k - 1] = isqrt_u64(val);
    }
    g_sarray = sarray;                    /* make it visible to the comparator */

    /* Create an array of indices 1..r to sort. */
    idx_t *indices = (idx_t*)malloc(r * sizeof(idx_t));
    if (!indices) {
        free(sarray);
        return 0;
    }
    for (idx_t i = 0; i < r; ++i) indices[i] = i + 1;

    /* Sort indices in descending order of the fractional parts. */
    qsort(indices, r, sizeof(idx_t), cmp_indices);

    seq_t result = (seq_t)indices[pos - 1]; /* pos is 1‑based */

    free(indices);
    free(sarray);
    g_sarray = NULL;

    return result;
}
