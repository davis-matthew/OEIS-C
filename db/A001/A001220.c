/*
 * A001220  Wieferich primes: primes p such that p^2 divides 2^(p-1) - 1.
 * https://oeis.org/A001220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, bref, nice, more
 * Formula:   search over the primes p in increasing order for those with
 *            2^(p-1) == 1 (mod p^2). Only a(1) = 1093 and a(2) = 3511 are
 *            known, and no third term exists below about 2^64. The search is
 *            capped (p^2 < 2^32, so the 64-bit modular products cannot
 *            overflow). An index past the known terms returns 0
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   a001220_isprime, a001220_wieferich
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a001220_isprime.h"
#include "../utils/a001220_wieferich.h"

seq_t a(idx_t n)
{
    uint64_t p;
    idx_t cnt = 0;
    for (p = 3; p < 65536; p += 2) {
        if (a001220_isprime(p) && a001220_wieferich(p)) {
            cnt++;
            if (cnt == n) return (seq_t)p;
        }
    }
    return (seq_t)0;
}
