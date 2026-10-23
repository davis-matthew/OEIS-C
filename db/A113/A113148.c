/*
 * A113148  Row 7 of table A113143; equal to INVERT of 7-fold factorials
 *          shifted one place right.
 * https://oeis.org/A113148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 * a(0) = 1
 * Let t(j) = product_{k=0}^{j-1} (7k + 1)   (with t(0) = 1)
 * Then for n >= 1
 *     a(n) = Σ_{j=0}^{n-1} t(j) * a(n-1-j)
 * 
 * This recurrence follows from the definition a(n+1)=Σ_{k=0}^{n} a(k)*A045754(n-k)
 * where A045754(k) = t(k) = ∏_{i=0}^{k-1} (7i+1).
 */

#include <stdlib.h>   /* malloc, free, etc. */
#include <stdint.h>   /* for uint64_t if needed */

/* ------------------------------------------------------------ */
/* Allocate and free arrays of seq_t in a way that works in C
 * and C++. For C++ the constructors/destructors are invoked
 * properly using new/delete. For C standard malloc/free are
 * used.                        */
#if defined(__cplusplus)
    #define ALLOC_ARRAY(T, N)  new T[(N)]
    #define FREE_ARRAY(A)      delete[] (A)
#else
    #define ALLOC_ARRAY(T, N)  ((T*)malloc(sizeof(T)*(N)))
    #define FREE_ARRAY(A)      free((A))
#endif
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate space for a[0..n] and t[0..n] */
    seq_t *a_arr = ALLOC_ARRAY(seq_t, n + 1);
    seq_t *t_arr = ALLOC_ARRAY(seq_t, n + 1);

    /* Base values */
    a_arr[0] = (seq_t)1;
    t_arr[0] = (seq_t)1;

    /* Precompute t(j) = product_{k=0}^{j-1} (7k + 1) */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t mult = ((seq_t)7 * (seq_t)(i - (idx_t)1)) + (seq_t)1;
        t_arr[i] = t_arr[i - (idx_t)1] * mult;
    }

    /* Compute a(n) using the recurrence.
     * a(m) = Σ_{j=0}^{m-1} t(j) * a(m-1-j)
     */
    for (idx_t m = (idx_t)1; m <= n; ++m)
    {
        seq_t sum = (seq_t)0;
        for (idx_t j = (idx_t)0; j < m; ++j)
        {
            sum += a_arr[m - (idx_t)1 - j] * t_arr[j];
        }
        a_arr[m] = sum;
    }

    seq_t result = a_arr[n];

    /* Free allocated memory */
    FREE_ARRAY(a_arr);
    FREE_ARRAY(t_arr);

    return result;
}
