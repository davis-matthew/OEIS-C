/*
 * A170657  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=24, a(n)=24*23^(n-1) for 2≤n≤48; For n≥49:
 *            a(n)=22*(a(n-1)+…+a(n-48))−253*a(n-49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    static seq_t *A = NULL;           /* array of computed terms */
    static size_t len = 0;            /* number of terms stored (max index+1) */
    static size_t cap = 0;            /* allocated capacity */

    /* if the requested term is already computed, just return it */
    if (n < len) return A[(size_t)n];

    /* ensure storage can hold up to index n */
    if (cap <= n) {
        size_t new_cap = (cap == 0) ? n + 1 : cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        A = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    size_t start = len;               /* first index to compute */
    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i == 0) {
            A[i] = 1;
        } else if (i <= 48) {
            if (i == 1) {
                A[i] = 24;
            } else {
                A[i] = A[i - 1] * 23;
            }
        } else { /* i >= 49 */
            seq_t sum = 0;
            for (size_t j = 1; j <= 48; ++j)
                sum += A[i - j];
            sum = sum * 22;
            sum -= A[i - 49] * 253;
            A[i] = sum;
        }
    }
    len = n + 1;                       /* update number of stored terms */
    return A[(size_t)n];
}
