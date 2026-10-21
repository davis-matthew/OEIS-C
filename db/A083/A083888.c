/*
 * A083888  Number of divisors of n with largest digit = 1 (base 10).
 * https://oeis.org/A083888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_divs_recursive, only01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A083888: a(n) = number of divisors d of n such that the
 * largest decimal digit of d is 1 (i.e. every digit of d is 0 or 1).
 *
 * Computation: factor n, enumerate all of its divisors, and count those
 * whose decimal representation contains only the digits 0 and 1.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_divs_recursive.h"
#include "../utils/only01.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined, but safe */
    uint64_t m = n;
    /* factor n; store primes and exponents */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    if (m % 2 == 0) {
        int e = 0;
        while ((m & 1) == 0) { m >>= 1; ++e; }
        primes[cnt] = 2; exps[cnt] = e; ++cnt;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[cnt] = p; exps[cnt] = e; ++cnt;
        }
    }
    if (m > 1) { primes[cnt] = m; exps[cnt] = 1; ++cnt; }

    seq_t result = 0;
    count_divs_recursive(0, 1, primes, exps, cnt, &result);
    return result;
}
