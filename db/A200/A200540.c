/*
 * A200540  Product of Pell and Motzkin numbers: a(n) =
 *          A000129(n+1)*A001006(n).
 * https://oeis.org/A200540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000129(n+1) * A001006(n) * * A000129 (Pell numbers):
 *            P(0)=0, P(1)=1, P(n)=2*P(n-1)+P(n-2) (n≥2) * A001006 (Motzkin
 *            numbers): M(0)=1, M(1)=1, * (n+2)·M(n) = (2n+1)·M(n-1) +
 *            (3n-3)·M(n-2) (n≥2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute Motzkin number M(n) = A001006(n)
 * Uses the recurrence: (n+2)M(n) = (2n+1)M(n-1) + (3n-3)M(n-2)
 * All arithmetic fits exactly in seq_t.
 */
static seq_t get_motzkin(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t m0 = (seq_t)1;   /* M(0) */
    seq_t m1 = (seq_t)1;   /* M(1) */
    seq_t m2;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t num   = (seq_t)(2*i + 1) * m1
                    + (seq_t)(3*i - 3) * m0;
        seq_t denom = i + 2;
        m2 = num / denom;   /* exact division */
        m0 = m1;
        m1 = m2;
    }
    return m1;   /* M(n) */
}

/* Compute Pell number P(n) = A000129(n)
 * n is an idx_t, but the recurrence uses arithmetic in seq_t.
 */
static seq_t get_pell(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t p0 = (seq_t)0;   /* P(0) */
    seq_t p1 = (seq_t)1;   /* P(1) */
    seq_t p2;
    for (idx_t i = 2; i <= n; ++i) {
        p2 = (seq_t)2 * p1 + p0;   /* P(i) = 2*P(i-1)+P(i-2) */
        p0 = p1;
        p1 = p2;
    }
    return p1;   /* P(n) */
}

/* Sequence function required by the build. */
seq_t a(idx_t n) {
    /* a(n) = Pell(n+1) * Motzkin(n) */
    seq_t pell = get_pell(n + (idx_t)1);
    seq_t motz = get_motzkin(n);
    return pell * motz;
}
