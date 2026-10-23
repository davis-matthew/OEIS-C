/*
 * A108149  Triangle T(n,k), 0<=k<=n, read by rows, defined by:
 *          T(0,0)=1,T(n,k)= 0 if n<k, T(n,k) = (-1)^n*T(n-1,k) + T(n-1,k-1) +
 *          T(n-1,k+1).
 * https://oeis.org/A108149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th entry of the triangle T(n,k) defined by *
 *            T(0,0) = 1, * T(n,k) = (-1)^n * T(n-1,k) + T(n-1,k-1) +
 *            T(n-1,k+1) (T(n,k)=0 if n<k), * and the sequence is read
 *            row‑wise (row 0: T(0,0), row 1: T(1,0),T(1,1), …)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#define ALLOC(T, N) reinterpret_cast<T*>(malloc((N) * sizeof(T)))
#else
#define ALLOC(T, N) (T*)malloc((N) * sizeof(T))
#endif

seq_t a(idx_t n) {
    /* Determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column within that row */
    idx_t c = n - r * (r + 1) / 2;

    /* Allocate space for two consecutive rows – size r+2 is enough for
       indices up to r+1 while computing row r. */
    size_t sz = r + 2;
    seq_t *prev = ALLOC(seq_t, sz);
    seq_t *curr = ALLOC(seq_t, sz);
    if (!prev || !curr) {       /* allocation failed – return 0 as safe default */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base row (row 0) */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t sign = (i & 1) ? (seq_t)-1 : (seq_t)1;

        for (idx_t k = 0; k <= i; ++k) {
            seq_t left   = (k > 0) ? prev[k - 1] : (seq_t)0;
            seq_t center = (k <= i - 1) ? prev[k] : (seq_t)0;
            seq_t right  = (k + 1 <= i - 1) ? prev[k + 1] : (seq_t)0;

            curr[k] = sign * center + left + right;
        }

        /* Swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c];

    free(prev);
    free(curr);
    return result;
}
