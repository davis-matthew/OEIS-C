/*
 * A182356  G.f. satisfies: A(x) = d/dx log(1+x + x^2*A(x)^2).
 * https://oeis.org/A182356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A182356 – a(n) = d/dx log(1 + x + x²*A(x)²)
   Recurrence (n ≥ 1):
     a₀ = 1
     let c_m = Σ_{i=0}^{m} a_i a_{m-i}      // coefficients of A²
     let e_m = Σ_{k=0}^{m} a_k c_{m-k}      // coefficients of A³
     let f_m = Σ_{k=0}^{m} a_k (m-k+1) a_{m-k+1}   // from A·A′
     then
         a_n = 2·c_{n-1} + 2·f_{n-2} – a_{n-1} – e_{n-2}
   The convolution c is maintained incrementally: when a_j is known,
   for k < j  :  c[j+k] += 2·a_j a_k  ;  c[2j] += a_j².
   Complexity:  O(n²) arithmetic operations, well below the 2‑second limit
   for n ≤ 10 000.                                            */

#include <stdlib.h>
#include <stdint.h>

/* helper to grow an array to at least 'need' elements            */
static void ensure_capacity(size_t need, seq_t **arr, size_t *cap)
{
    if (*cap <= need) {
        size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out‑of‑memory */
        for (size_t i = *cap; i < new_cap; ++i) tmp[i] = 0;
        *arr   = tmp;
        *cap   = new_cap;
    }
}

/* external linkage – the required function                     */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *A = NULL;          /* a[]             */
    static seq_t *C = NULL;          /* convolution A²  */
    static size_t capA = 0;          /* size of A[]     */
    static size_t capC = 0;          /* size of C[]     */
    static idx_t last = 0;           /* largest index computed  */

    /* allocate/resize if needed                               */
    ensure_capacity((size_t)n, &A, &capA);
    ensure_capacity((size_t)(2 * (size_t)n + 1), &C, &capC);

    /* initialise first term on first call                     */
    if (A[0] == 0 && C[0] == 0) {    /* A[0] is guaranteed 0 if never set      */
        A[0] = 1;
        C[0] = A[0] * A[0];          /* =1                                       */
        last = 0;
    }

    /* compute missing terms up to n                            */
    for (idx_t j = last + 1; j <= n; ++j) {
        seq_t f = 0;
        seq_t e = 0;

        /* f_{j-2}  (only if j ≥ 2)                               */
        if (j >= 2) {
            for (idx_t k = 0; k <= j - 2; ++k) {
                /* (j-1-k) is small – cast to seq_t safely           */
                seq_t temp = (seq_t)(j - 1 - k) * A[j - 1 - k];
                f += A[k] * temp;
            }
        }

        /* e_{j-2}  (only if j ≥ 2)                               */
        if (j >= 2) {
            for (idx_t k = 0; k <= j - 2; ++k) {
                e += A[k] * C[(j - 2) - k];
            }
        }

        /* compute a_j                                 */
        seq_t a_j = ( (seq_t)2 * C[j - 1] )          /* 2*c_{j-1}       */
                  + ( (seq_t)2 * f )                /* 2*f_{j-2}       */
                  - A[j - 1]                        /* -a_{j-1}        */
                  - e;                              /* -e_{j-2}        */
        A[j] = a_j;

        /* update convolution C with new term a_j                    */
        for (idx_t k = 0; k < j; ++k) {
            seq_t prod = A[j] * A[k];
            /* indices j+k satisfy j+k <= 2*j-1  (k < j)   */
            C[j + k] += prod * (seq_t)2;      /* double for i≠j   */
        }
        /* diagonal term a_j * a_j at index 2*j                  */
        C[2 * j] += A[j] * A[j];
    }

    last = n;
    return A[n];
}
