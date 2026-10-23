/*
 * A120792  Numerators of partial sums of Catalan numbers scaled by powers of
 *          -1/12.
 * https://oeis.org/A120792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of Sum_{k=0..n} (-1)^k * C(k)/12^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C(k) is the Catalan number; a(0)=1.  */
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* N_k = Sum_{i=0..k} (-1)^i * C(i) * 12^{k-i}  */
    seq_t N = (seq_t)1;          /* N_0 */
    const seq_t C12 = (seq_t)12; /* constant 12 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t cat = catalan(i);
        seq_t tmp = N * C12;    /* 12 * N_{i-1} */

        if (i & 1)             /* i odd: subtract C(i) */
            N = tmp - cat;
        else                   /* i even: add C(i)  */
            N = tmp + cat;
    }

    /* Reduce N by the common factor 2^a * 3^b with the denominator 12^n = 2^{2n} * 3^n. */
    idx_t exp2_cap = 2 * n;
    idx_t exp3_cap = n;

    /* Remove powers of 2 up to exp2_cap.  */
    while ((N & (seq_t)1) == 0 && exp2_cap > 0) {
        N >>= 1;
        exp2_cap--;
    }

    /* Remove powers of 3 up to exp3_cap.  */
    while ((N % (seq_t)3) == 0 && exp3_cap > 0) {
        N /= 3;
        exp3_cap--;
    }

    return N;
}
