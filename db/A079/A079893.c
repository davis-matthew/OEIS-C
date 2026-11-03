/*
 * A079893  a(n) = gcd(n, A079892(n)), where A079892(n) is the least number >
 *          n having one more distinct prime factor than n.
 * https://oeis.org/A079893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(n, m) where m is the least integer > n having one
 *            more distinct prime factor than n
 * Range:     u8 n <= 328, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 9588 (2026-11-01)
 * Helpers:   gcd, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, so n>=1 is required */

    int dn = omega((uint64_t)n);
    idx_t m = n + 1;
    for (;;) {
        if (omega((uint64_t)m) == dn + 1)
            break;
        ++m;
    }
    seq_t sn = (seq_t)n;
    seq_t sm = (seq_t)m;
    return gcd(sn, sm);
}
