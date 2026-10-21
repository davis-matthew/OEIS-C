/*
 * A210282  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          determinant n.
 * https://oeis.org/A210282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p=n}^{n²} A(p)·A(p-n) where A(p) = |{(x,y)∈[0,n]×[0,n]
 *            : x·y = p}|. This counts all 2×2 matrices with entries 0…n and
 *            determinant n
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t N = n;
    /* maximum product */
    unsigned long long prod_max_ll = (unsigned long long)N * (unsigned long long)N;
    size_t sz = (size_t)prod_max_ll + 1;          /* array size for products */

    /* allocate array of 16‑bit counters */
    uint16_t *A = (uint16_t *)calloc(sz, sizeof(uint16_t));
    if (!A)
        return (seq_t)0;                          /* allocation failure -> 0 */

    /* A[0] = 2·N + 1, counts (0,any) and (any,0) minus the double count at (0,0) */
    if (prod_max_ll >= 0)   /* always true, but keeps the compiler quiet about unused code */
        A[0] = (uint16_t)(2ULL * N + 1);

    /* Count pairs (a,d) with product p (excluding zeros already counted) */
    for (idx_t a = 1; a <= N; ++a) {
        for (idx_t d = 1; d <= N; ++d) {
            idx_t p = a * d;          /* 1 ≤ p ≤ prod_max_ll */
            ++A[p];
        }
    }

    /* Compute the answer using the convolution of A with itself shifted by n */
    seq_t result = 0;
    for (idx_t p = N; p <= prod_max_ll; ++p) {
        result += (seq_t)A[p] * (seq_t)A[p - N];
    }

    free(A);
    return result;
}
