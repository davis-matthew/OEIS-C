/*
 * A061118  Let s(n) be the sequence of squares (A000290). Then this sequence
 *          is given by s(1), s(2)s(1)s(3), s(4)s(2)s(1)s(3)s(5), ...
 * https://oeis.org/A061118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concatenation of squares s(2(n-1)), s(2(n-2)), …, s(2),
 *            s(1), s(3), …, s(2n-1), where s(k) = k². For n=1 this yields 1,
 *            for n=2 → 419, for n=3 → 1641925, …
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"       /* pow10(idx_t) */
#include "../utils/digit_len.h"   /* digit_len(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0)          /* Offset is 1, but guard against 0. */
        return 0;

    seq_t res = 0;

    /* Append even squares in descending order: 2(n-1), 2(n-2), …, 2 */
    for (idx_t i = n - 1; i >= 1; --i) {
        idx_t idx = i * 2;              /* even index 2*i */
        seq_t sq   = (seq_t)idx * (seq_t)idx;
        seq_t shift = pow10((idx_t)digit_len(sq));
        res = res * shift + sq;
    }

    /* Append odd squares in ascending order: 1, 3, 5, …, 2n-1 */
    for (idx_t i = 1; i <= 2 * n - 1; i += 2) {
        seq_t sq   = (seq_t)i * (seq_t)i;
        seq_t shift = pow10((idx_t)digit_len(sq));
        res = res * shift + sq;
    }

    return res;
}
