/*
 * A047666  Square array a(n,k) read by antidiagonals: a(n,1)=n, a(1,k)=k,
 *          a(n,k) = a(n-1,k-1) + a(n-1,k) + a(n,k-1).
 * https://oeis.org/A047666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   alloc_seq_array, idx_uv, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047666: Square array a(n,k) read by antidiagonals
   Recurrence:
      a(n,1) = n
      a(1,k) = k
      a(n,k) = a(n‑1,k‑1) + a(n‑1,k) + a(n,k‑1)   (n,k > 1)
   The sequence a(n) is obtained by reading the square array along
   anti‑diagonals (OEIS offset 1). */
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/idx_uv.h"

static seq_t *A = NULL;      /* table stored row‑major, 1‑based indices */
static idx_t   A_max = 0;     /* maximum dimension stored */

static void ensure_table(idx_t tmax)
{
    if (tmax <= A_max)
        return;                       /* already large enough */

    /* allocate new table of size (tmax+1)*(tmax+1) */
    size_t sz = (size_t)(tmax + 1) * (tmax + 1);
    seq_t *B = alloc_seq_array(sz);
    if (!B)
        return;                       /* allocation failed – ignore */

    /* set boundary values: a(1,k)=k, a(n,1)=n */
    for (idx_t i = 1; i <= tmax; ++i) {
        B[idx_uv(1, i, tmax)] = (seq_t)i;
        B[idx_uv(i, 1, tmax)] = (seq_t)i;
    }

    /* interior values using the recurrence */
    for (idx_t i = 2; i <= tmax; ++i) {
        for (idx_t j = 2; j <= tmax; ++j) {
            seq_t v = B[idx_uv(i - 1, j - 1, tmax)] +
                      B[idx_uv(i - 1, j,     tmax)] +
                      B[idx_uv(i,     j - 1, tmax)];
            B[idx_uv(i, j, tmax)] = v;
        }
    }

    if (A)
        seq_free_array(A);
    A = B;
    A_max = tmax;
}

static idx_t find_diag(idx_t pos)
{
    if (pos <= 1)
        return 2;                       /* first diagonal contains it (n=1) */

    idx_t low = 2, high = 3;
    while ((high * (high - 1)) / 2 < pos)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid - 1)) / 2 >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined below offset */

    /* locate (i,j) corresponding to the n‑th entry */
    idx_t t    = find_diag(n);                 /* diagonal number (t ≥ 2) */
    idx_t prev = (t - 2) * (t - 1) / 2;        /* elements before this diag */
    idx_t p    = n - prev;                     /* position inside diag (1..t‑1) */
    idx_t i    = t - p;                        /* row index (1‑based) */
    idx_t j    = p;                            /* column index (1‑based) */

    /* ensure table covers required indices */
    idx_t needed = i > j ? i : j;
    ensure_table(needed);

    return A[idx_uv(i, j, A_max)];
}
