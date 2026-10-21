/*
 * A082050  Sum of divisors of n that are not of the form 3k+1.
 * https://oeis.org/A082050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d%3!=1} d (sum of divisors of n not ≡1 mod 3)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/dfs_rec.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n<1, return 0 */

    /* Factorization: simple trial division */
    uint64_t temp = n;
    uint64_t pr[64];
    int exps[64];
    int pc = 0;

    int e = 0;
    while ((temp & 1ULL) == 0) { temp >>= 1; ++e; }
    if (e > 0) {
        pr[pc] = 2;
        exps[pc] = e;
        ++pc;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        e = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        if (e > 0) {
            pr[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (temp > 1) {
        pr[pc] = temp;
        exps[pc] = 1;
        ++pc;
    }

    seq_t sum = 0;
    dfs_rec(pr, exps, pc, 0, (seq_t)1, &sum);
    return sum;
}
