/*
 * A079893  a(n) = gcd(n, A079892(n)), where A079892(n) is the least number >
 *          n having one more distinct prime factor than n.
 * https://oeis.org/A079893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(n, m) where m is the least integer > n having one
 *            more distinct prime factor than n
 * Range:     u8 n <= 328, u16 n <= 9588, u32 n <= 9588, u64 n <= 9588,
 *            u128 n <= 9588, big any n
 * Helpers:   count_distinct_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, so n>=1 is required */

    int dn = count_distinct_primes((uint64_t)n);
    idx_t m = n + 1;
    for (;;) {
        if (count_distinct_primes((uint64_t)m) == dn + 1)
            break;
        ++m;
    }
    seq_t sn = (seq_t)n;
    seq_t sm = (seq_t)m;
    return gcd(sn, sm);
}
