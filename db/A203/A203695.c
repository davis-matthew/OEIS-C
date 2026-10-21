/*
 * A203695  Vandermonde sequence using x^2 + y^2 applied to the first n
 *          triangular numbers: 1,3,6,10,...,n(n+1)/2.
 * https://oeis.org/A203695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ i < j ≤ n} (T_i^2 + T_j^2), where T_k = k(k+1)/2
 *            (the k‑th triangular number)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th term of sequence A203695,
   computing a(1)=1, a(2)=10, a(3)=16650, ... */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *a_cache = NULL;   /* a_cache[1..] holds a(n) */
static seq_t *tri = NULL;       /* tri[1..] holds T_n */
static idx_t cache_size = 0;    /* highest n that has been computed */

seq_t a(idx_t n)
{
    /* no need to handle n < 1; OEIS offset is 1 */
    if (n == 0)
        return (seq_t)0;

    /* grow cache if necessary */
    if (n > cache_size)
    {
        /* allocate more space for a_cache and tri */
        size_t new_size = (size_t)(n + 1);   /* indices 0..n, 0 unused */
        seq_t *new_a = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        seq_t *new_tri = (seq_t *)realloc(tri, new_size * sizeof(seq_t));
        if (!new_a || !new_tri)
            exit(1);  /* allocation failed */
        a_cache = new_a;
        tri = new_tri;
        /* initialise missing entries */
        /* compute triangular numbers and a(n) incrementally */
        for (idx_t k = cache_size + 1; k <= n; ++k)
        {
            tri[k] = ((seq_t)k * ((seq_t)k + 1)) / 2;
            if (k == 1)
            {
                a_cache[1] = (seq_t)1;
                continue;
            }
            /* start with product from previous term */
            seq_t prod = a_cache[k - 1];
            seq_t ti = tri[k];
            for (idx_t j = 1; j < k; ++j)
            {
                seq_t tj = tri[j];
                prod *= (tj * tj + ti * ti);
            }
            a_cache[k] = prod;
        }
        cache_size = n;
    }
    return a_cache[n];
}
