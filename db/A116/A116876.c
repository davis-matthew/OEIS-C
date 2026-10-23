/*
 * A116876  Generalized Catalan numbers C(2,6;n)=C(6,2;n).
 * https://oeis.org/A116876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the recurrence * a(0)=1,\; a(1)=1, *
 *            5*a(n)+32*a(n-1)+12*a(n-2)=84*Catalan(n-2)*12^(n-2) for n≥2 *
 *            where Catalan(k) is the usual Catalan number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A116876 using the recurrence above.
 * idx_t and seq_t types are provided by the build environment.                 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Pre‑compute Catalan numbers C[0..n-2] */
    size_t limit = (size_t)n - 1;        /* n-1 elements, last index is n-2   */
    seq_t *catalan = (seq_t *)malloc(limit * sizeof(seq_t));
    if (catalan == NULL) return (seq_t)0; /* in case of allocation failure */

    catalan[0] = (seq_t)1;                 /* C0 = 1 */
    for (idx_t k = 1; k <= n - 2; ++k) {
        /* C_k = C_{k-1} * 2*(2*(k-1)+1) / (k+1) */
        seq_t num   = catalan[k - 1] * (seq_t)(2 * (2 * (k - 1) + 1));
        seq_t denom = (seq_t)(k + 1);
        catalan[k] = num / denom;         /* division is exact */
    }

    /* Pre‑compute powers of 12: 12^e for e=0..n-2 */
    seq_t *pow12 = (seq_t *)malloc(limit * sizeof(seq_t));
    if (pow12 == NULL) { free(catalan); return (seq_t)0; }

    pow12[0] = (seq_t)1;
    for (idx_t e = 1; e <= n - 2; ++e) {
        pow12[e] = pow12[e - 1] * (seq_t)12;
    }

    /* Recurrence to compute a(n) iteratively */
    seq_t a_nm2 = (seq_t)1; /* a(0)   */
    seq_t a_nm1 = (seq_t)1; /* a(1)   */
    seq_t a_n   = a_nm1;    /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t rhs = catalan[i - 2];
        rhs *= (seq_t)84;
        rhs *= pow12[i - 2];

        seq_t numerator = rhs - (seq_t)32 * a_nm1 - (seq_t)12 * a_nm2;
        a_n = numerator / (seq_t)5;        /* division is exact */

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    free(catalan);
    free(pow12);
    return a_n;
}
