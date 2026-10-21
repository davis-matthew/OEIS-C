/*
 * A228745  Expansion of (phi(q)^4 + 7 * phi(-q)^4) / 8 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A228745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -6 * b(n) where b is multiplicative: b(0) = 1 b(2^e) = -4
 *            (e >= 1) For odd prime powers p^e: b(p) = p + 1 b(p^e) =
 *            b(p^(e-1)) * (p + 1) - p * b(p^(e-2)) (e >= 2) a(0) = 1
 * Range:     i8 n <= 9, i16 n <= 1469, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    seq_t bn = 1;                   /* b(n) product over prime powers */
    idx_t m = n;

    /* trial division for prime factors up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        seq_t term;
        if (p == 2) {
            /* b(2^e) = -4 for any e >= 1 */
            term = (seq_t)-4;
        } else {
            /* compute b(p^e) by recurrence */
            seq_t bp = (seq_t)p + 1;   /* b(p^1) */
            if (e == 1) {
                term = bp;
            } else {
                seq_t b_prev_prev = (seq_t)1; /* b(p^0) */
                seq_t b_prev = bp;            /* b(p^1) */
                seq_t b_curr = 0;
                for (idx_t i = 2; i <= e; ++i) {
                    b_curr = b_prev * bp - (seq_t)p * b_prev_prev;
                    b_prev_prev = b_prev;
                    b_prev = b_curr;
                }
                term = b_curr;
            }
        }
        bn = bn * term;
    }

    /* residue prime factor > 1 */
    if (m > 1) {
        idx_t p = m;
        seq_t term;
        if (p == 2) {
            term = (seq_t)-4;
        } else {
            term = (seq_t)p + 1;
        }
        bn = bn * term;
    }

    return -6 * bn;
}
