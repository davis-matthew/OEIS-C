/*
 * A046970  Dirichlet inverse of the Jordan function J_2 (A007434).
 * https://oeis.org/A046970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 184, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046970 – Dirichlet inverse of the Jordan function J_2
   Formula: a(n) = Σ_{d|n} μ(d)·d²
   (multiplicative: a(p^e) = 1 – p² for p prime, e ≥ 1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t res = 1;
    idx_t m = n;

    /* Handle factor 2 separately (avoids the even step in the loop) */
    if (m % 2 == 0) {
        seq_t sq = (seq_t)2 * (seq_t)2;
        res *= ((seq_t)1 - sq);
        while (m % 2 == 0) m /= 2;
    }

    /* Trial division by odd numbers */
    for (idx_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            seq_t sq = (seq_t)d * (seq_t)d;
            res *= ((seq_t)1 - sq);
            while (m % d == 0) m /= d;
        }
    }

    /* Remaining prime factor > 1 */
    if (m > 1) {
        seq_t sq = (seq_t)m * (seq_t)m;
        res *= ((seq_t)1 - sq);
    }

    return res;
}
