/*
 * A131203  Number of cycles of length n under the mapping x -> x^2-2 modulo
 *          Fermat prime 2^(2^m)+1, where m is any fixed integer such that n
 *          divides 2^m-1.
 * https://oeis.org/A131203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001037(2n+1)/2 * where A001037(k) = (1/k) * Σ_{d|k} μ(d)
 *            * 2^{k/d} (Möbius μ) * * The implementation computes this
 *            directly for odd k = 2n+1. * It generates prime numbers up to
 *            20001 once, factors k, enumerates * all divisors, accumulates
 *            positive and negative contributions * according to...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Prime table ------------------------------------------------------------ */
static uint32_t *prime_list = NULL; /* array of primes <= 20001 */
static size_t  prime_count  = 0;    /* number of primes */

/* Generate primes up to 20001 (once) -------------------------------------*/
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    const int MAX = 20001;
    bool *sieve = (bool *)malloc((MAX + 1) * sizeof(bool));
    if (!sieve) return; /* safety, although unlikely */

    for (int i = 0; i <= MAX; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (int i = 2; i * i <= MAX; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX; j += i) sieve[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (int i = 2; i <= MAX; ++i) if (sieve[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) { free(sieve); return; }

    size_t idx = 0;
    for (int i = 2; i <= MAX; ++i) if (sieve[i]) prime_list[idx++] = (uint32_t)i;
    prime_count = cnt;

    free(sieve);
}

/* Recursive divisor enumeration ------------------------------------------*/
static void dfs_divisors(int pos, uint64_t cur_d, int mu, int has_sq,
                         seq_t *pos_sum, seq_t *neg_sum,
                         idx_t k,
                         const uint32_t *primes, const int *exps, int fac_cnt)
{
    if (pos == fac_cnt) {
        if (has_sq) return;                   /* μ(d) = 0, skip */
        idx_t exp = k / cur_d;                /* exponent in 2^{k/d} */
        seq_t term = pow2(exp);               /* 2^{k/d} */
        if (mu == 1) *pos_sum += term;
        else          *neg_sum += term;       /* μ = -1 */
        return;
    }

    uint32_t p = primes[pos];
    int  emax = exps[pos];

    for (int e = 0; e <= emax; ++e) {
        uint64_t nd = cur_d;
        if (e > 0) nd *= p;
        int nmu   = mu;
        int nhas  = has_sq;
        if (e >= 2) nhas = 1;          /* squared prime factor -> μ = 0 */
        if (e == 1) nmu = -mu;          /* flip sign for each prime once */
        dfs_divisors(pos + 1, nd, nmu, nhas, pos_sum, neg_sum,
                     k, primes, exps, fac_cnt);
    }
}

/* Main function ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure prime list is ready */
    init_primes();

    /* k = 2n + 1 (always odd) */
    idx_t k = (idx_t)2 * n + (idx_t)1;

    /* Factor k: store distinct primes and their exponents */
    uint32_t fac_primes[16];
    int      fac_exp[16];
    int      fac_cnt = 0;

    uint64_t temp = k;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fac_primes[fac_cnt] = p;
            fac_exp[fac_cnt]    = e;
            ++fac_cnt;
        }
    }
    if (temp > 1) {
        fac_primes[fac_cnt] = (uint32_t)temp;
        fac_exp[fac_cnt]    = 1;
        ++fac_cnt;
    }

    /* Enumerate all divisors, accumulate positive and negative sums */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    dfs_divisors(0, 1ULL, 1, 0, &pos_sum, &neg_sum,
                 k, fac_primes, fac_exp, fac_cnt);

    /* Final division by 2k */
    seq_t denom = (seq_t)(2ULL * k);
    seq_t result = (pos_sum - neg_sum) / denom;

    return result;
}
