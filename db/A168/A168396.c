/*
 * A168396  Triangle, T(n,k) = number of compositions a(1),...,a(j) of n with
 *          a(1) = k, such that a(i+1) <= a(i) + 1 for 1 <= i < j.
 * https://oeis.org/A168396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) is the triangle from A168396 defined
 *            by T(1,1)=1, T(n,k)=1 for k=n, T(n,k)=sum_{x=1}^{min(k+1,n-k)}
 *            T(n-k,x) otherwise
 * Range:     u8 n <= 78, u16 n <= 253, u32 n <= 903, u64 n <= 3403,
 *            u128 n <= 7260, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is produced by flattening the triangle row‑major
   (a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"   /* for seq_alloc */

static seq_t **tri = NULL;      /* pointer to rows, 1‑based */
static idx_t tri_rows = 0;      /* highest row computed */
static idx_t tri_cap  = 0;      /* capacity of tri array */

/* Make sure rows 1..r are computed. */
static void ensure_rows(idx_t r)
{
    if (r <= tri_rows) return;          /* already ready */

    /* Grow the pointer array if needed. */
    idx_t new_cap = tri_cap ? tri_cap : 1;
    while (new_cap <= r) new_cap <<= 1;
    tri = (seq_t **)realloc(tri, new_cap * sizeof(seq_t *));
    for (idx_t i = tri_cap; i < new_cap; ++i) tri[i] = NULL;
    tri_cap = new_cap;

    /* Build new rows one by one. */
    for (idx_t i = tri_rows + 1; i <= r; ++i) {
        /* Allocate space for indices 0..i (we ignore index 0). */
        size_t sz = (size_t)i + 1;
        tri[i] = seq_alloc(sz);      /* seq_alloc constructs objects */

        for (idx_t k = 1; k <= i; ++k) {
            if (k == i) {
                tri[i][k] = 1;       /* diagonal element */
            } else {
                seq_t sum = 0;
                idx_t part = i - k;
                idx_t max_x = part;
                if (max_x > k + 1) max_x = k + 1;
                for (idx_t x = 1; x <= max_x; ++x) {
                    sum += tri[part][x];
                }
                tri[i][k] = sum;
            }
        }
    }
    tri_rows = r;
}

/* Return a(n) – the nth term of the flattened triangle. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but returns 0 */

    /* Find row r such that (r-1)*r/2 < n <= r*(r+1)/2. */
    idx_t low = 1, high = 1;
    while (((__int128)high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri_mid = ((__int128)mid * (mid + 1)) / 2;
        if (tri_mid >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column inside this row */
    __int128 before128 = ((__int128)r * (r - 1)) / 2;
    idx_t before = (idx_t)before128;
    idx_t k = n - before;

    /* Ensure the triangle rows are available. */
    ensure_rows(r);

    return tri[r][k];
}
