/*
 * A218089  a(n) = n*((n+1)^n - n^(n-1)).
 * https://oeis.org/A218089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*((n+1)^n - n^(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t np1_seq = (seq_t)(n + 1);

    // (n+1)^n
    seq_t pow1 = ipow(np1_seq, n_seq);

    // n^(n-1)
    seq_t pow2 = ipow(n_seq, (seq_t)(n - 1));

    // n*((n+1)^n - n^(n-1))
    return n_seq * (pow1 - pow2);
}
