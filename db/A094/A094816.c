/*
 * A094816  Triangle read by rows: T(n,k) are the coefficients of Charlier
 *          polynomials: A046716 transposed, for 0 <= k <= n.
 * https://oeis.org/A094816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n-th entry of the triangle T(n,k) given by *
 *            T(n+1,k) = (n+1)*T(n,k) + T(n,k-1) - n*T(n-1,k); * T(0,0)=1,
 *            T(0,k>0)=0, T(n,k<0)=0. * a(n) is obtained by reading the
 *            triangle row‑by‑row (offset 0). The * implementation computes
 *            the required row of the triangle on the fly. * No memoisati...
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>   /* For placement new on arbitrary‑precision types */
#endif

seq_t a(idx_t n)
{
    /* Special‑case the very first element. */
    if (n == 0)
        return (seq_t)1;

    /* Find the row m that contains index n.
       The first index of row m is start = m*(m+1)/2. */
    idx_t m    = 0;
    idx_t start = 0;
    while (1) {
        idx_t next_start = start + (m + 1);
        if (next_start > n)
            break;
        m = m + 1;
        start = next_start;
    }
    /* Position within row m. */
    idx_t k = n - start;

    /* Allocate three rows of maximum required length (m+1). */
    size_t maxrow = (size_t)(m + 1);
    seq_t *row_prev  = (seq_t *)malloc(maxrow * sizeof(seq_t));
    seq_t *row_prev2 = (seq_t *)malloc(maxrow * sizeof(seq_t));
    seq_t *row_curr  = (seq_t *)malloc(maxrow * sizeof(seq_t));
    if (!row_prev || !row_prev2 || !row_curr)
        abort();   /* Out of memory */

#ifdef __cplusplus
    /* Construct all elements for a class type. */
    for (size_t i = 0; i < maxrow; ++i) {
        new (&row_prev[i])  seq_t();
        new (&row_prev2[i]) seq_t();
        new (&row_curr[i])  seq_t();
    }
#endif

    /* Initialise row 0: only T(0,0)=1. */
    row_prev[0] = (seq_t)1;
    /* Zero remaining slots to avoid reading uninitialised data. */
    for (size_t i = 1; i < maxrow; ++i) {
        row_prev[i]  = (seq_t)0;
        row_prev2[i] = (seq_t)0;
        row_curr[i]  = (seq_t)0;
    }

    if (m == 0) {
        seq_t result = row_prev[0];
        free(row_prev);
        free(row_prev2);
        free(row_curr);
        return result;
    }

    /* Compute rows 1 … m using the recurrence. */
    for (idx_t i = 1; i <= m; ++i) {
        idx_t len = i + 1;   /* Entries 0 … i */
        for (idx_t j = 0; j <= i; ++j) {
            seq_t a_val = (j <= i - 1)     ? row_prev[j]    : (seq_t)0;
            seq_t b_val = (j >= 1)         ? row_prev[j - 1] : (seq_t)0;
            seq_t c_val = (j <= i - 2)     ? row_prev2[j]   : (seq_t)0;
            seq_t term1 = (seq_t)i * a_val;
            seq_t term2 = b_val;
            seq_t term3 = (seq_t)(i - 1) * c_val;
            row_curr[j] = term1 + term2 - term3;
        }
        /* Rotate the row pointers for the next iteration. */
        seq_t *tmp = row_prev2;
        row_prev2 = row_prev;
        row_prev  = row_curr;
        row_curr  = tmp;
    }

    /* Extract the required entry from the completed row. */
    seq_t result = row_prev[k];

    /* Clean up */
    free(row_prev);
    free(row_prev2);
    free(row_curr);

    return result;
}
