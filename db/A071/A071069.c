/*
 * A071069  a(n) = min { A070923(k) | n^3 < k < (n+1)^3 }.
 * https://oeis.org/A071069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min { A070923(k) : n^3 < k < (n+1)^3 }, where A070923(k)
 *            = ceil(k^(2/3))^3 - k^2; computed by direct search over k
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   icbrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/icbrt_seq.h"

static seq_t A070923_a(seq_t nseq)
{
    seq_t n2 = nseq * nseq;
    seq_t k = icbrt_seq(n2);
    if (k * k * k < n2)
        k++;
    return k * k * k - n2;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t lo = nn * nn * nn + 1;
    seq_t hi = (nn + 1) * (nn + 1) * (nn + 1);
    seq_t best = 0;
    bool have = false;
    seq_t k;
    for (k = lo; k < hi; k++) {
        seq_t v = A070923_a(k);
        if (!have || v < best) {
            best = v;
            have = true;
        }
    }
    return best;
}
