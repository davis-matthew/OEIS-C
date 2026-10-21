/*
 * A084867  Symmetric square table, read by antidiagonals, such that
 *          antidiagonal sums form the first row shifted left: T(0,0)=1,
 *          T(0,k) = Sum_{m=0..k-1} T(m,k-1-m) when k > 0; and T(n,k) =
 *          T(n-1,k) + T(n,k-1) when n > 0, k > 0.
 * https://oeis.org/A084867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084867 – Symmetric square table, read by antidiagonals.
   Table T(n,k) is defined by
        T(0,0)=1,
        T(0,k)=4*T(0,k-1)-2*T(0,k-2)  for k≥2,
        T(n,k)=T(n-1,k)+T(n,k-1)      for n>0 and k>0.
   The sequence a(n) is the n‑th element of the table read along the
   antidiagonals (i+j=n).  The algorithm memoises partial tables up to
   the largest diagonal requested.  It uses only the permitted operators
   and works for both fixed‑width unsigned integer types and for
   arbitrary‑precision types (where `seq_t` is a C++ class).  The only
   allocation functions are `malloc`/`realloc`, cast to `seq_t*`
   so they are valid in C and C++.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for incremental memoisation */
    static idx_t max_d = 0;          /* highest diagonal calculated so far   */
    static seq_t *A = NULL;          /* boundary values T(0,k), size=max_d+1 */
    static seq_t *T = NULL;          /* flattened table: (max_d+1)x(max_d+1) */

    /* Find the diagonal d that contains index n.
       Total entries up to (and including) diagonal d-1 is d*(d+1)/2.
       The while loop stops with the smallest d such that
       (d+1)*(d+2)/2 > n, so n lies on diagonal d. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Ensure tables are large enough.  If no tables exist, create
       them.  Otherwise grow them only if required. */
    if (T == NULL)          /* first call – allocate everything */
    {
        size_t sizeA = (size_t)(d + 1);
        size_t sizeT = sizeA * sizeA;

        A = (seq_t*)malloc(sizeA * sizeof(seq_t));
        if (!A)
            return (seq_t)0;        /* out of memory – caller must ignore */
        T = (seq_t*)malloc(sizeT * sizeof(seq_t));
        if (!T)
        {
            free(A);
            return (seq_t)0;
        }

        /* Compute boundary sequence A(k)=T(0,k). */
        A[0] = (seq_t)1;
        if (d >= 1)
            A[1] = (seq_t)1;
        for (idx_t k = 2; k <= d; ++k)
            A[k] = (seq_t)(4 * A[k - 1] - 2 * A[k - 2]);

        /* Fill the full table using the recurrence. */
        for (idx_t i = 0; i <= d; ++i)
        {
            for (idx_t j = 0; j <= d; ++j)
            {
                size_t idx = (size_t)i * (d + 1) + j;
                if (i == 0 && j == 0)
                {
                    T[idx] = A[0];
                }
                else if (i == 0)
                {
                    T[idx] = A[j];
                }
                else if (j == 0)
                {
                    T[idx] = A[i];
                }
                else
                {
                    T[idx] = T[(size_t)i * (d + 1) + (j - 1)] +
                             T[(size_t)(i - 1) * (d + 1) + j];
                }
            }
        }
        max_d = d;
    }
    else if (d > max_d)     /* grow tables only if the needed diagonal
                               is larger than the current maximum */
    {
        size_t sizeA = (size_t)(d + 1);
        size_t sizeT = sizeA * sizeA;

        A = (seq_t*)realloc(A, sizeA * sizeof(seq_t));
        if (!A)
            return (seq_t)0;
        T = (seq_t*)realloc(T, sizeT * sizeof(seq_t));
        if (!T)
        {
            free(A);
            return (seq_t)0;
        }

        /* Recompute boundary values up to the new diagonal. */
        A[0] = (seq_t)1;
        if (d >= 1)
            A[1] = (seq_t)1;
        for (idx_t k = 2; k <= d; ++k)
            A[k] = (seq_t)(4 * A[k - 1] - 2 * A[k - 2]);

        /* Rebuild the entire table with the larger dimensions. */
        for (idx_t i = 0; i <= d; ++i)
        {
            for (idx_t j = 0; j <= d; ++j)
            {
                size_t idx = (size_t)i * (d + 1) + j;
                if (i == 0 && j == 0)
                {
                    T[idx] = A[0];
                }
                else if (i == 0)
                {
                    T[idx] = A[j];
                }
                else if (j == 0)
                {
                    T[idx] = A[i];
                }
                else
                {
                    T[idx] = T[(size_t)i * (d + 1) + (j - 1)] +
                             T[(size_t)(i - 1) * (d + 1) + j];
                }
            }
        }
        max_d = d;
    }

    /* Determine the (i,j) coordinates of index n on diagonal d. */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;
    idx_t j = d - offset;
    size_t idx = (size_t)i * (max_d + 1) + j;
    return T[idx];
}
