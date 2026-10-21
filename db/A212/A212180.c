/*
 * A212180  Number of distinct second signatures (cf. A212172) represented
 *          among divisors of n.
 * https://oeis.org/A212180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct second signatures among divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_recurse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/dfs_recurse.h"

/* factorise a uint64_t n into prime bases and exponents (max 16 factors) */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps,
                          size_t *cnt)
{
    size_t c = 0;
    if (n == 0) { *cnt = 0; return; }

    if (n % 2 == 0) {
        uint64_t e = 0;
        while (n % 2 == 0) { e++; n /= 2; }
        primes[c] = 2;
        exps[c]  = e;
        ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { e++; n /= p; }
            primes[c] = p;
            exps[c]  = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* public function as required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behaviour for indices < 1 is irrelevant */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[16], exps[16];
    size_t num_factors = 0;
    factor_uint64(nn, primes, exps, &num_factors);

    uint64_t cur_exp[16] = {0};

    size_t cap = 4;
    size_t cnt = 0;
    char **signatures = (char **)malloc(cap * sizeof(char *));
    if (!signatures) return 0;

    dfs_recurse(0, num_factors, exps, cur_exp,
                &signatures, &cnt, &cap);

    for (size_t i = 0; i < cnt; ++i) free(signatures[i]);
    free(signatures);

    return (seq_t)cnt;
}
