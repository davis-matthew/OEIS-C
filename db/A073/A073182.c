/*
 * A073182  Number of divisors of n which are not greater than the cubefree
 *          kernel of n.
 * https://oeis.org/A073182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are <= the cubefree kernel
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/dfs_count.h"

seq_t a(idx_t n) {
    /* Factorisation of n (trial division is sufficient for n ≤ 10000). */
    uint64_t primes[16];
    uint8_t exp[16];
    size_t cnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exp[cnt] = 1;
        ++cnt;
    }

    /* Compute the cubefree kernel: product of p^min(e,2). */
    uint64_t kernel = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint8_t mul = (exp[i] < 2 ? exp[i] : 2);
        for (uint8_t j = 0; j < mul; ++j)
            kernel *= primes[i];
    }

    /* Count divisors not exceeding the kernel. */
    uint64_t divcnt = 0;
    dfs_count(0, 1, primes, exp, cnt, kernel, &divcnt);

    return (seq_t)divcnt;
}
