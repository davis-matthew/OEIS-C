/*
 * A043512  Numbers having four 5's in base 10.
 * https://oeis.org/A043512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _enum_rec, _enumerate_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043512: Numbers having exactly four 5's in base 10.
    The function generates the n‑th such number by enumerating
    all k‑digit numbers (k ≥ 4) in ascending order until the
    desired index is reached. */

#include <stdbool.h>
#include "../utils/_enum_rec.h"
#include "../utils/_enumerate_k.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* the sequence is 1‑based; undefined for 0 */
        return 0;
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t k = 4; ; ++k) {
        _enumerate_k(k, &count, n, &result);
        if (count >= n)
            return result;
    }
}
