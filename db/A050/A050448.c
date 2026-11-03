/*
 * A050448  a(n) = Sum_{d|n, d==1 (mod 4)} d^4.
 * https://oeis.org/A050448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d ≡ 1 (mod 4)} d^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 8902 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* -------------------  Recursive divisor summation  ------------------- */
static void dfs_sum(const uint64_t *fac_primes, const int *exps,
                    int fac_cnt, int idx,
                    seq_t cur_prod, int cur_mod4,
                    seq_t *sum)
{
    if (idx == fac_cnt) {
        if (cur_mod4 == 1) {
            seq_t p2 = cur_prod * cur_prod;
            seq_t p4 = p2 * p2;
            *sum += p4;
        }
        return;
    }

    uint64_t p = fac_primes[idx];
    int        e = exps[idx];

    /* Divisors containing 2 cannot be ≡ 1 (mod 4); skip all exponents > 0 */
    if (p == 2) {
        dfs_sum(fac_primes, exps, fac_cnt, idx + 1, cur_prod, cur_mod4, sum);
        return;
    }

    seq_t prod = cur_prod;
    for (int k = 0; k <= e; ++k) {
        int new_mod4;
        if ((p & 3ULL) == 1UL)                 /* p ≡ 1 (mod 4) */
            new_mod4 = cur_mod4;
        else if ((p & 3ULL) == 3ULL)           /* p ≡ 3 (mod 4) */
            new_mod4 = (cur_mod4 * ((k & 1) ? 3 : 1)) & 3;
        else                                   /* should not happen */
            new_mod4 = cur_mod4;

        dfs_sum(fac_primes, exps, fac_cnt, idx + 1, prod, new_mod4, sum);

        if (k < e)
            prod = prod * p;
    }
}

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
