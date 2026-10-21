/*
 * A217436  Triangular array read by rows. T(n,k) is the number of labeled
 *          relations on n elements with exactly k vertices of indegree and
 *          outdegree = 0.
 * https://oeis.org/A217436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 20, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A217436
    a(n) is the n‑th term of the triangular array read by rows.
    The array T(r,k) satisfies                T(r,k)=binom(r,k)*A(r-k)
    where A(m) is the number of labeled relations on m elements with
    no isolated vertices.  A(m) is computed by the recurrence
        A(0)=1,
        A(m)=2^(m^2) – Σ_{j=0}^{m-1} binom(m,j)*A(j)   (m>0).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) -> seq_t   */
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) -> seq_t */

static seq_t *A_arr = NULL;   /* cache for A(m) values   */
static idx_t   A_len = 0;     /* number of stored A values (next free index) */

/* Extend the cache so that it contains A(0)..A(max_n). */
static void ensure_A(idx_t max_n)
{
    if (A_len > max_n) return;                     /* already enough */

    /* Allocate or grow the array.  realloc(NULL, size) is equivalent to malloc. */
    seq_t *new_arr = (seq_t *)realloc(A_arr, (size_t)(max_n + 1) * sizeof(seq_t));
    if (!new_arr) exit(1);                          /* fatal allocation failure */
    A_arr = new_arr;

    /* Compute the missing A values. */
    for (idx_t n = A_len; n <= max_n; ++n) {
        seq_t val;
        if (n == 0) {
            val = (seq_t)1;                          /* A(0)=1 */
        } else {
            seq_t total = pow_uint(2, (idx_t)n * (idx_t)n); /* 2^(n^2) */
            seq_t sum   = 0;
            for (idx_t j = 0; j < n; ++j) {
                seq_t bin  = binomial(n, j);
                sum += bin * A_arr[j];
            }
            val = total - sum;                       /* A(n) */
        }
        A_arr[n] = val;
    }
    A_len = max_n + 1;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Determine row (r) and column (k) for flattened index n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;       /* r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t k = n - r * (r + 1) / 2;                   /* column index */

    /* Compute A values needed for the binomial term. */
    ensure_A(r);                                      /* we need A up to r-k <= r */

    seq_t bin  = binomial(r, k);
    seq_t prod = bin * A_arr[r - k];                  /* T(r,k) = binomial(r,k) * A(r-k) */

    return prod;
}
