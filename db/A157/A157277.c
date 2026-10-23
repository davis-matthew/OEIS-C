/*
 * A157277  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) + m*f(n,k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1, f(n, k) = 2*k if k <= floor(n/2) otherwise 2*(n-k), and
 *          m = 2, read by rows.
 * https://oeis.org/A157277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k,2) = (2*(n-k)+1)*T(n-1,k-1,2) + (2*k+1)*T(n-1,k,2) +
 *            2*f(n,k)*T(n-2,k-1,2), where f(n,k) = 2*k if k <= floor(n/2)
 *            2*(n-k) otherwise. Boundary: T(n,0,2) = T(n,n,2) = 1. The
 *            sequence a(n) is obtained by reading the triangle row‑wise, i.e.
 *            a(n) = T(r,k,2) with n = r*(r+1)/2 + k
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 159,
 *            u128 n <= 445, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
  #include <new>                      /* needed for operator new[] */
  #define ALLOC_ROW(sz)  new seq_t[(sz)]
  #define FREE_ROW(ptr)  delete[] ptr
#else
  #define ALLOC_ROW(sz)  ((seq_t*)malloc((sz)*sizeof(seq_t)))
  #define FREE_ROW(ptr)  free(ptr)
#endif

seq_t a(idx_t n)
{
    /* Handle the trivial first term */
    if (n == 0) return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    seq_t *prev2 = NULL;   /* row r-2  */
    seq_t *prev1 = NULL;   /* row r-1  */
    seq_t *curr  = NULL;   /* row r    */

    /* Build rows from 0 up to r */
    for (idx_t i = 0; i <= r; ++i) {
        size_t sz = (size_t)(i + 1);
        curr = ALLOC_ROW(sz);
        if (!curr) {        /* allocation failure – return 0 */
            FREE_ROW(prev2);
            FREE_ROW(prev1);
            return (seq_t)0;
        }

        for (idx_t j = 0; j <= i; ++j) {
            if (j == 0 || j == i) {
                curr[(size_t)j] = (seq_t)1;
            } else {
                /* Values from previous rows */
                seq_t a_val = prev1[(size_t)(j - 1)];
                seq_t b_val = prev1[(size_t)j];
                seq_t c_val = (i >= 2) ? prev2[(size_t)(j - 1)] : (seq_t)0;

                /* f(i,j) */
                idx_t f_val = (j <= i / 2) ? (2 * j) : (2 * (i - j));

                seq_t term_a = ((seq_t)2 * (i - j) + (seq_t)1) * a_val;
                seq_t term_b = ((seq_t)2 * j + (seq_t)1) * b_val;
                seq_t term_c = ((seq_t)2 * f_val) * c_val;

                curr[(size_t)j] = term_a + term_b + term_c;
            }
        }

        /* Advance rows for next iteration */
        FREE_ROW(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    /* prev1 now points to row r */
    seq_t result = prev1[(size_t)k];

    /* Clean up remaining rows */
    FREE_ROW(prev2);
    FREE_ROW(prev1);

    return result;
}
