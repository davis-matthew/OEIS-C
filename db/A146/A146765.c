/*
 * A146765  Triangle read by rows: expansion of p(x,n)=If[n == 0, 1, (x + 1)^n
 *          + 2^(n - 1)*Sum[Binomial[n, m]*x^m*(1 + x^(n - 2*m)), {m, 1, n -
 *          1}]].
 * https://oeis.org/A146765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   Let r be the unique non‑negative integer with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and let c = n – r(r+1)/2. Then the triangle entry
 *            is a(n) = 1 if c = 0 or c = r a(n) = C(r, c) * (1 + 2^r)
 *            otherwise
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t offset = 0;
    /* Find row r such that offset ≤ n < offset + (r+1) */
    while (true) {
        idx_t row_len = r + 1;
        if (n < offset + row_len) {
            break;
        }
        ++r;
        offset += r;          /* offset becomes r(r+1)/2 after the increment */
    }

    idx_t c = n - offset;

    /* Edge cases at the ends of each row */
    if (c == 0 || c == r) {
        return (seq_t)1;
    }

    seq_t bin = binom(r, c);      /* C(r, c) as seq_t */
    seq_t two = pow2(r);          /* 2^r as seq_t */
    return bin * (two + (seq_t)1);/* C(r, c) * (1 + 2^r) */
}
