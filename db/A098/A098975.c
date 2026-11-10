/*
 * A098975  Nonzero elements of table A071675; also counts selected ordered
 *          multisets of the values {1,2,3}.
 * https://oeis.org/A098975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = nonzero terms of A071675 in antidiagonal order; entry
 *            (i,j) = [x^j](1+x+x^2)^i = Sum_{c=max(0,j-i)..floor(j/2)}
 *            C(i,c)*C(i-c,j-2c), antidiagonal s=i+j, i from ceil(s/3) to s
 * Range:     u8 n <= 54, u16 n <= 173, u32 n <= 583, u64 n <= 883,
 *            u128 n <= 883, big any n
 * Validated: standard, pass, 86 terms through n = 874 (2026-11-07)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t rem = n;
    idx_t s = 0;
    for (;;) {
        idx_t cnt = s - (s + 2) / 3 + 1;
        if (rem < cnt) break;
        rem -= cnt;
        s++;
    }
    idx_t i = (s + 2) / 3 + rem;
    idx_t j = s - i;
    idx_t start_c = (j > i) ? (j - i) : 0;
    idx_t end_c = j / 2;
    seq_t result = 0;
    for (idx_t c = start_c; c <= end_c; ++c) {
        result += binomial(i, c) * binomial(i - c, j - 2 * c);
    }
    return result;
}
