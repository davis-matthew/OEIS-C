/*
 * A050448  a(n) = Sum_{d|n, d==1 (mod 4)} d^4.
 * https://oeis.org/A050448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d ≡ 1 (mod 4)} d^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/dfs_sum.h"

/* -------------------  Prime sieve (up to 1 000 000)  ------------------- */
#define MAX_PRIME 1000000
static uint64_t primes[MAX_PRIME];
static size_t primes_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool *is_composite = (bool*)calloc(MAX_PRIME + 1, sizeof(bool));
    for (uint64_t p = 2; p * p <= MAX_PRIME; ++p) {
        if (!is_composite[p]) {
            for (uint64_t q = p * p; q <= MAX_PRIME; q += p)
                is_composite[q] = true;
        }
    }
    for (uint64_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_composite[i]) primes[primes_cnt++] = i;
    free(is_composite);
    primes_initialized = true;
}

/* -------------------  Main sequence function  ------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* seq is defined for n ≥ 1 */

    init_primes();

    /* Factor n */
    idx_t tmp = n;
    uint64_t fac_primes[64];
    int       exps[64];
    int       fac_cnt = 0;

    if ((tmp & 1) == 0) {          /* factor 2 */
        int e = 0;
        while ((tmp & 1) == 0) { tmp >>= 1; ++e; }
        fac_primes[fac_cnt] = 2;
        exps[fac_cnt++]     = e;
    }

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            fac_primes[fac_cnt] = p;
            exps[fac_cnt++]     = e;
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        fac_primes[fac_cnt] = tmp;
        exps[fac_cnt++]     = 1;
    }

    seq_t sum = 0;
    dfs_sum(fac_primes, exps, fac_cnt, 0, (seq_t)1, 1, &sum);
    return sum;
}
