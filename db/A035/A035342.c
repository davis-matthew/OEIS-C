/*
 * A035342  The convolution matrix of the double factorial of odd numbers
 *          (A001147).
 * https://oeis.org/A035342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let T(k)=k*(k+1)/2.  For n≥1 find k such that T(k‑1)<n≤T(k) and
   m=n−T(k‑1).  Then a(n)=a(k,m) where
        a(1,1)=1,   a(n,0)=0,   a(n,m)=0 for m>n,
        a(n+1,m) = (2*n + m)*a(n,m) + a(n,m‑1).
   The sequence is the triangle a(n,m) listed row by row: 1; 3,1; 15,9,1; ... */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *tri_buffer = NULL;      /* buffer storing one triangle row */
static size_t  buffer_cap = 0;        /* current capacity of tri_buffer */

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* safety, sequence starts at offset 1 */

    /* Find row index k (1‑based) such that T(k‑1) < n ≤ T(k) */
    idx_t k = 0;
    while (((__int128)k * (k + 1)) / 2 < n) ++k;   /* k grows until condition holds */

    /* Column index m (1‑based) within that row */
    idx_t prevT = (idx_t)(((__int128)k * (k - 1)) / 2);
    idx_t m = n - prevT;

    /* Allocate buffer large enough to hold one row up to length k */
    size_t need = (size_t)(k + 1);
    if (buffer_cap < need) {
        seq_t *tmp = (seq_t*)realloc(tri_buffer, need * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        tri_buffer = tmp;
        buffer_cap = need;
    }
    seq_t *row = tri_buffer;

    /* Initialize row: a[0] = 0; a[1] = 1 initially */
    row[0] = 0;
    for (size_t i = 1; i <= (size_t)k; ++i)
        row[i] = 0;
    row[1] = 1;                        /* a(1,1) */

    /* Build up to row k using the recurrence */
    for (idx_t i = 2; i <= k; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            row[j] = ((seq_t)(2 * (i - 1) + j) * row[j]) + row[j - 1];
        }
    }

    return row[m];
}
