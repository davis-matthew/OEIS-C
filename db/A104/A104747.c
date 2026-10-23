/*
 * A104747  a(n) = (n-3)*2^n + n*(n+3)/2 + 3.
 * https://oeis.org/A104747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*2^n + n*(n+3)/2 + 3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1, harmless */

    const seq_t three = (seq_t)3;
    seq_t n_seq = (seq_t)n;
    seq_t pow2n = pow2(n);                    /* 2^n */

    /* n*(n+3)/2  (works for all n) */
    seq_t half = n_seq * (n_seq + three) / 2;

    if (n <= 3) {
        /* a(n) = n*(n+3)/2 + 3 - (3-n)*2^n   (3-n is non‑negative here) */
        seq_t sub_factor = three - n_seq;      /* 3-n, fits in seq_t */
        seq_t sub = sub_factor * pow2n;        /* (3-n)*2^n */
        return half + three - sub;             /* no wrap, result ≥ 0 */
    } else {
        /* a(n) = (n-3)*2^n + n*(n+3)/2 + 3 */
        seq_t term_a = (n_seq - three) * pow2n;
        return term_a + half + three;
    }
}
