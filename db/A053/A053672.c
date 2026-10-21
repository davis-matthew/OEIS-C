/*
 * A053672  Least number coprime to n, n+1, n+2 and n+3.
 * https://oeis.org/A053672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = minimum integer m ≥ 2 such that gcd(m, n + i) = 1 for all
 *            i = 0,1,2,3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* Euclidean gcd for unsigned 64‑bit integers. */

seq_t a(idx_t n) {
    uint64_t n0 = (uint64_t)n;      /* safe copy of the index */
    for (uint64_t m = 2; ; ++m) {
        if (gcd64(m, n0)       != 1) continue;
        if (gcd64(m, n0 + 1u)   != 1) continue;
        if (gcd64(m, n0 + 2u)   != 1) continue;
        if (gcd64(m, n0 + 3u)   != 1) continue;
        return (seq_t)m;
    }
}
