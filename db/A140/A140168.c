/*
 * A140168  Triangle read by rows, iterates of X * [1,0,0,0,...]; where X = an
 *          infinite bidiagonal matrix with [2, -1, 2, -1, 2, ...] in the main
 *          diagonal, [1, 1, 1, ...] in the subdiagonal and rest zeros.
 * https://oeis.org/A140168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140168:  Triangle read by rows.  The r‑th row is obtained by
   applying the bidiagonal matrix X to the previous row.  The matrix
   X has main diagonal entries 2, –1 alternating, subdiagonal entries
   1, and zeros elsewhere:

              2  0  0  0  …
            –1  2  0  0  …
              1 –1  2  0  …
              0  1 –1  2  …
              ⋮           

   The vector e₀ = [1,0,0,…] is iterated,
   and the triangle T is read row–wise.  The function a(n) returns
   the n‑th entry in this triangle, with a(0)=1.  */

#include <stdlib.h>

#ifdef __cplusplus
  #include <new>
#endif

/* allocate an array of seq_t, null‑initialised if seq_t is a trivial
   type; works with both C++ (class type) and C (plain integer). */
static seq_t *allocate_seq_arr(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* free an array allocated by allocate_seq_arr */
static void free_seq_arr(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* binary search for the largest r such that r*(r+1)/2 <= n.
   Uses 128–bit arithmetic to avoid overflow. */
static idx_t triangular_row(idx_t n) {
    idx_t low = 0;
    idx_t high = n + 1;        /* exclusive upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* compute mid*(mid+1)/2 in 128 bits */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;            /* r with T_r <= n < T_{r+1} */
}

seq_t a(idx_t n) {
    /* Determine the row r and column j within that row. */
    idx_t r = triangular_row(n);
    idx_t tri = (idx_t)r * (r + 1) / 2;   /* T_r fits in idx_t */
    idx_t j = n - tri;                    /* 0 ≤ j ≤ r */

    /* Special case: first row */
    if (r == 0) {
        seq_t one = 1;
        return one;
    }

    /* Allocate arrays for the previous and current rows. */
    seq_t *prev = allocate_seq_arr(1);
    prev[0] = 1;      /* row 0 */

    for (idx_t step = 1; step <= r; ++step) {
        seq_t *curr = allocate_seq_arr(step + 1);

        /* First element: doubles each step (diag=2, no subdiag). */
        curr[0] = prev[0] * 2;

        for (idx_t k = 1; k <= step; ++k) {
            seq_t prev_k = (k <= step - 1) ? prev[k] : 0;
            seq_t prev_km1 = prev[k - 1];

            if ((k & 1) == 0) {          /* even k: diag = +2 */
                curr[k] = prev_k * 2 + prev_km1;
            } else {                     /* odd k: diag = –1 */
                curr[k] = prev_km1 - prev_k;
            }
        }

        free_seq_arr(prev);
        prev = curr;
    }

    seq_t result = prev[j];
    free_seq_arr(prev);
    return result;
}
