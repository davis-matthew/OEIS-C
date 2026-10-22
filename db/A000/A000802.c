/*
 * A000802  Maximal number of states in the minimal deterministic finite
 *          automaton accepting a language over a binary alphabet consisting
 *          of some words of length n.
 * https://oeis.org/A000802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} min(2^k, 2^(2^(n-k))-1)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   a000802_pow2pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000802_pow2pow2.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t p = 1; /* 2^k */
    idx_t k;
    for (k = 0; k <= n; k++) {
        idx_t e = n - k;
        /* 2^k < 2^(2^e) - 1  iff  k < 2^e */
        if (e >= 63 || k < ((idx_t)1 << e)) {
            result += p;
        } else {
            result += a000802_pow2pow2(e) - 1;
        }
        p = p + p;
    }
    return result;
}
