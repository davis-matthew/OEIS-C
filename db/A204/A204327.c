/*
 * A204327  a(n) = Pell(n^2).
 * https://oeis.org/A204327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n^2) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

/* -- Matrix structure for Pell’s recurrence --------------------------- */
typedef struct {
    seq_t a, b, c, d;          /* matrix [[a b],[c d]] */
} _matrix_t;

/* Matrix multiplication over seq_t (no extra casts required). */
static inline _matrix_t _mul_mat(_matrix_t m1, _matrix_t m2) {
    _matrix_t r;
    r.a = m1.a * m2.a + m1.b * m2.c;
    r.b = m1.a * m2.b + m1.b * m2.d;
    r.c = m1.c * m2.a + m1.d * m2.c;
    r.d = m1.c * m2.b + m1.d * m2.d;
    return r;
}

/* Compute the k‑th Pell number: P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2). */
static seq_t _pell(idx_t k) {
    /* Base matrix for the Pell recurrence:
       M = [[2,1],[1,0]] so that M^k = [[P(k+1),P(k)],[P(k),P(k-1)]]     */
    _matrix_t base = { (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)0 };
    _matrix_t res  = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };   /* identity */

    for (idx_t e = k; e > 0; e >>= 1) {
        if (e & 1) res = _mul_mat(res, base);
        base = _mul_mat(base, base);
    }
    /* The (2,1) entry of M^k is the Pell number P(k). */
    return res.c;
}

/* Public entry point for the OEIS sequence A204327. */
seq_t a(idx_t n) {
    /* n ≥ 1 by the offset; the behavior for n < 1 is unspecified.   */
    idx_t k = n * n;           /* exponent for Pell numbers */
    return _pell(k);
}
