/*
 * A210561  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210562; see the Formula section.
 * https://oeis.org/A210561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   Triangle T(n,k) of coefficients of u(n,x), a(n) is the flattened
 *            row-major order of T(n,k). T(1,1)=1, T(n,1)=1 for n>1, T(n,k) =
 *            2*T(n-1,k-1) + T(n-2,k-1) (n>=2, k>=2)
 * Range:     u8 n <= 34, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types are provided by the build system */
#include <stdlib.h>   /* malloc, free, memset */

seq_t a(idx_t n)
{
    /* sequence is 1-indexed; ignore n==0 */
    if (n == 0) return 0;

    /* Find minimal row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) r++;

    /* Column within row r */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t k = n - prev_tri;      /* 1-based column */

    /* Allocate triangular array of size (r+1) x (r+1) */
    size_t size = (size_t)(r + 1) * (size_t)(r + 1);
    seq_t *T = (seq_t *)malloc(size * sizeof(seq_t));
    if (!T)
        return 0;                /* fallback on allocation failure */

    /* Initialize to zero */
    memset(T, 0, size * sizeof(seq_t));

    /* Base cases */
    T[1 * (r + 1) + 1] = 1;      /* T(1,1) */

    /* Fill rows 2..r */
    for (idx_t i = 2; i <= r; ++i) {
        T[i * (r + 1) + 1] = 1;  /* first column always 1 */
        for (idx_t j = 2; j <= i; ++j) {
            seq_t t1 = T[(i - 1) * (r + 1) + (j - 1)];
            seq_t t2 = T[(i - 2) * (r + 1) + (j - 1)];
            T[i * (r + 1) + j] = t1 + t1 + t2;  /* 2*t1 + t2 */
        }
    }

    /* Retrieve answer */
    seq_t res = T[r * (r + 1) + k];

    /* Clean up */
    free(T);
    return res;
}
