/*
 * A097471  First differences of A076678.
 * https://oeis.org/A097471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A076678(n) - A076678(n-1), where A076678(0) = A076678(1)
 *            = 1, A076678(k) = floor(sqrt(A076678(k-1))) + A076678(k-2)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-07)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)1;   /* A076678(0) */
    seq_t c = (seq_t)1;   /* A076678(1) */
    if (n <= 1)
        return (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = isqrt_seq(c) + p;
        if (i == n)
            return next - c;
        p = c;
        c = next;
    }
    return (seq_t)0;
}
