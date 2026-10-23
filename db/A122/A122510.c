/*
 * A122510  Array T(d,n) = number of integer lattice points inside the
 *          d-dimensional hypersphere of radius sqrt(n), read by ascending
 *          antidiagonals.
 * https://oeis.org/A122510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A122510: Number of integer lattice points inside the
   d‑dimensional hypersphere of radius sqrt(m), read by ascending
   antidiagonals. 
   For an index n ≥ 1 the function first recovers the pair
   (d,m) such that a(n) = T(d,m), where
          T(d,m) = |{ (x1,…,xd) ∈ ℤ^d : Σ xi² ≤ m }|.
   The mapping from n to (d,m) is:

        s         = smallest integer with s(s+1)/2 ≥ n
        k         = n - s(s-1)/2          (1 ≤ k ≤ s)
        d         = s - k + 1              (1 ≤ d ≤ s)
        m         = k - 1                  (0 ≤ m ≤ s-1)

   T(d,m) is computed by dynamic programming:

        T(0,v) = 1 for all v ≥ 0
        T(d,v) = Σ_{t² ≤ v}   c(t) * T(d-1, v - t²)   with
                                   c(0)=1, c(t)=2 for t≠0.

   The algorithm uses only integer arithmetic and works for any
   implementation of seq_t (primitive integer or arbitrary‑precision
   class) and idx_t (uint64_t). */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* map the index n to (d,m) */
    idx_t s = 1;
    while (s * (s + 1) / 2 < n) ++s;
    idx_t prev_sum = s * (s - 1) / 2;
    idx_t k = n - prev_sum;           /* 1 ≤ k ≤ s            */
    idx_t d = s - k + 1;               /* 1 ≤ d ≤ s            */
    idx_t m = k - 1;                   /* 0 ≤ m ≤ s-1          */

    /* dynamic programming arrays for dimensions 0 … d */
    size_t sz = (size_t)(m + 1);
#ifdef __cplusplus
    seq_t *prev = new seq_t[sz];
    seq_t *curr = new seq_t[sz];
#else
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
#endif

    /* T(0,v) = 1 for all v */
    for (idx_t i = 0; i <= m; ++i) prev[i] = 1;

    /* iterate over dimensions 1 … d */
    for (idx_t dim = 1; dim <= d; ++dim) {
        for (idx_t v = 0; v <= m; ++v) {
            seq_t sum = 0;
            for (idx_t t = 0; t * t <= v; ++t) {
                seq_t val = prev[v - t * t];
                if (t == 0)
                    sum += val;
                else {
                    sum += val;
                    sum += val;
                }
            }
            curr[v] = sum;
        }
        /* swap buffers */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[m];

    /* free allocated memory */
#ifdef __cplusplus
    delete[] prev;
    delete[] curr;
#else
    free(prev);
    free(curr);
#endif

    return result;
}
