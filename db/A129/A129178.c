/*
 * A129178  Triangle read by rows: T(n,k) is the number of permutations p of
 *          {1,2,...,n} such that invc(p)=k (n >= 0; 0 <= k <= (n-1)(n-2)/2)
 *          (see comment for invc definition).
 * https://oeis.org/A129178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the coefficient of t^k in the polynomial * P[n](t) where
 *            P[1](t)=1, P[2](t)=2 and for n≥3, *
 *            P[n](t)=P[n‑1](t)·(2+t+...+t^(n‑2)). * The index n is
 *            interpreted by reading the triangle of the * sequence by rows:
 *            a(n)=T(row,offset) where row and offset are * found by
 *            accumulating row lengths
 * Range:     u8 n <= 28, u16 n <= 100, u32 n <= 398, u64 n <= 1434,
 *            u128 n <= 6783, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#include <cstdint>
#endif

/* Helper to allocate an array of seq_t objects.
   In C++ we use new[], in C we use malloc. */
static seq_t *alloc_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the triangle row r and column k that contains the
       flattened index n.  */
    idx_t      r    = 0;            /* current row being examined   */
    idx_t      acc  = 0;            /* cumulative terms processed   */

    while (1)
    {
        size_t len_r;                /* length of current row        */
        if (r == 0 || r == 1)
            len_r = 1;
        else
            len_r = 1 + (size_t)((idx_t)(r-1) * (idx_t)(r-2)) / 2;

        if (acc + (idx_t)len_r > n)
            break;                  /* n lies in this row           */

        acc += (idx_t)len_r;
        ++r;
    }

    idx_t k = n - acc;               /* column within row r, 0‑based */

    /* Rows 0,1 and 2 are trivial. */
    if (r <= 1)
        return (seq_t)1;
    if (r == 2)
        return (seq_t)2;

    /* Build polynomials up to row r.  Previous row polynomial
       is kept in 'prev' of length len_prev. */
    seq_t *prev = alloc_array(1);    /* P[2](t) = [2] */
    prev[0] = (seq_t)2;
    size_t len_prev = 1;

    for (idx_t row = 3; row <= r; ++row)
    {
        idx_t deg     = row - 2;                     /* degree of factor (n-2) */
        size_t new_len = len_prev + deg;            /* polynomial length */
        seq_t *cur = alloc_array(new_len);

        /* Initialise all coefficients to zero. */
        for (size_t i = 0; i < new_len; ++i)
            cur[i] = 0;

        /* Multiply previous polynomial by factor
           (2 + t + ... + t^deg). */
        for (idx_t i = 0; i < (idx_t)len_prev; ++i)
        {
            cur[i] += prev[i] * (seq_t)2; /* constant term 2 */
            for (idx_t j = 1; j <= deg; ++j)
                cur[i + j] += prev[i];   /* coefficient 1 for t^j */
        }

        free_array(prev);
        prev     = cur;
        len_prev = new_len;
    }

    /* Result is coefficient at position k in the final polynomial. */
    seq_t result = prev[(size_t)k];
    free_array(prev);
    return result;
}
