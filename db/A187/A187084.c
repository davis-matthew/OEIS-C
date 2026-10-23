/*
 * A187084  Exponential Riordan array (1,4*x+6*x^2+4*x^3+x^4).
 * https://oeis.org/A187084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry B(r,k) of the exponential Riordan array
 *            (1,4x+6x^2+4x^3+x^4), * where r is the smallest integer with
 *            r(r+1)/2 ≥ n and k = n – r(r−1)/2 (1‑based). * The array entries
 *            satisfy * B(0,0)=1, B(n,0)=0 (n>0), * for k≥2: B(n,k) = (1/k) *
 *            Σ_{m=1..4} s_m * (n!/(n−m)!) * B(n−m,k−1), * wher...
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Index of the first element of row r in the 1‑D array (row 0 occupies index 0). */
static size_t row_offset(idx_t r)
{
    return (r == 0 ? 0UL : 1UL + (r * (r - 1)) / 2);
}

/* Static cache for triangular entries B(n,k). */
static seq_t *B_cache = NULL;        /* flattened array B[n][k]  (k≥1 for n≥1) */
static idx_t  B_max_row = 0;         /* highest row computed so far */
static size_t B_cache_entries = 0;   /* total number of elements stored */

/* Ensure that rows 0 … r (inclusive) are computed. */
static void ensure_rows(idx_t r)
{
    if (r <= B_max_row)
        return;

    size_t new_entries = 1 + (r * (r + 1)) / 2;

    if (B_cache == NULL)
        B_cache = (seq_t *)malloc(new_entries * sizeof(seq_t));
    else
        B_cache = (seq_t *)realloc(B_cache, new_entries * sizeof(seq_t));

    if (!B_cache)
        abort();

    for (size_t i = B_cache_entries; i < new_entries; ++i)
        B_cache[i] = (seq_t)0u;
    B_cache_entries = new_entries;

    if (B_max_row == 0)
        B_cache[0] = (seq_t)1;      /* B(0,0) = 1 */

    const idx_t S[] = {4, 6, 4, 1};

    for (idx_t n = B_max_row + 1; n <= r; ++n) {
        size_t offset_n = row_offset(n);

        for (idx_t k = 1; k <= n; ++k) {
            seq_t numer = 0;

            if (k == 1) {  /* special handling for k = 1 */
                if (n <= 4) {
                    seq_t fact = 1;
                    for (idx_t t = 2; t <= n; ++t)
                        fact *= (seq_t)t;
                    numer = (seq_t)S[n - 1] * fact;
                }  /* else numer stays 0 for n > 4 */
            } else {  /* k >= 2 : use recurrence */
                for (idx_t m = 1; m <= 4; ++m) {
                    if (n < m)
                        continue;
                    if ((k - 1) > (n - m))
                        continue;

                    seq_t prod = (seq_t)S[m - 1];
                    seq_t p = 1;
                    for (idx_t t = 0; t < m; ++t)
                        p *= (seq_t)(n - t);
                    prod *= p;

                    size_t offset_nm = row_offset(n - m);
                    seq_t prev = B_cache[offset_nm + (k - 2)];
                    prod *= prev;

                    numer += prod;
                }
            }

            B_cache[offset_n + (k - 1)] = numer / (seq_t)k;  /* division exact */
        }
    }

    B_max_row = r;
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Find smallest r with r(r+1)/2 >= n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    idx_t prev = (r - 1) * r / 2;          /* elements before row r */
    idx_t k   = n - prev;                  /* 1‑based column index */

    ensure_rows(r);

    size_t offset = row_offset(r);
    return B_cache[offset + (k - 1)];
}
