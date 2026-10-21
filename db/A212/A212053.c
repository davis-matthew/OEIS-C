/*
 * A212053  Number of (w,x,y,z) with all terms in {1,...,n} and w*x<=y*z+1.
 * https://oeis.org/A212053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (w,x,y,z) with w,x,y,z ∈ [1,n] and w*x ≤ y*z +
 *            1. Computation uses the identity a(n) = n⁴ – Σ_{t=1}^{n²}
 *            C(t)·S(t–2), where C(t) = |{(i,j) : i·j = t}| and S(m) =
 *            Σ_{k=1}^{m} C(k)
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Allocate an array for the counts of products.  */
    static uint16_t *C = NULL;
    static size_t cap = 0;
    idx_t N = n * n;                     /* maximum product value */

    if (N + 1 > cap) {
        /* Grow array to the required size.  */
        if (C) free(C);
        C = (uint16_t*)calloc((size_t)(N + 1), sizeof(uint16_t));
        cap = (size_t)(N + 1);
        if (!C) return 0;                /* allocation failed, return 0 as a fallback */
    } else {
        /* Reuse the array and clear the part we will use.  */
        memset(C, 0, (size_t)(N + 1) * sizeof(uint16_t));
    }

    /* Count pairs (i,j) with product equal to t.  */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            ++C[i * j];
        }
    }

    /* Compute the summed term Σ C(t)·S(t–2).  */
    seq_t sum1 = 0;      /* result of the sum */
    seq_t curS = 0;      /* Σ_{k=1}^{t-2} C(k) for current t */
    uint16_t prevC = 0;  /* C(t-1) from the previous iteration */

    for (idx_t t = 1; t <= N; ++t) {
        sum1 += (seq_t)C[t] * curS;
        curS += prevC;          /* prepare curS for next t: add C(t-1) */
        prevC = C[t];
    }

    /* Final answer a(n) = n⁴ – sum1.  */
    seq_t nseq = (seq_t)n;
    seq_t total = nseq * nseq * nseq * nseq;
    return total - sum1;
}
