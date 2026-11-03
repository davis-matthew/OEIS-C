/*
 * A053473  a(n) is the cototient of n (A051953) iterated 5 times.
 * https://oeis.org/A053473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - φ(n)) iterated 5 times (the 5‑fold cototient of n)
 * Range:     u8 n <= 1468, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9896 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    for (int i = 0; i < 5; ++i) {
        if (v == 0)            /* once zero is reached it remains zero      */
            return v;
        seq_t p = totient((idx_t)v);   /* φ(v), using the helper provided in phi.h */
        v -= p;                    /* cototient: v - φ(v)                       */
    }
    return v;
}
