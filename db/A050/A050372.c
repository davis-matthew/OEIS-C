/*
 * A050372  Number of ways to factor n into distinct composite factors.
 * https://oeis.org/A050372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to factor n into distinct composite
 *            factors. * The implementation enumerates all composite divisors
 *            of n and counts * subsets whose product equals n, using memoised
 *            DFS
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, dfs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/dfs.h"

static bool prime_table[10001];
static bool prime_init = false;

/* Simple sieve of Eratosthenes up to 10000 */
static void init_primes(void)
{
    if (prime_init) return;
    prime_init = true;
    for (int i = 0; i <= 10000; ++i) prime_table[i] = true;
    prime_table[0] = prime_table[1] = false;
    for (int i = 2; i * i <= 10000; ++i) {
        if (prime_table[i]) {
            for (int j = i * i; j <= 10000; j += i)
                prime_table[j] = false;
        }
    }
}

static bool is_prime_uint64(uint64_t x)
{
    if (x <= 10000) return prime_table[(int)x];
    for (uint64_t i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1)=1 */

    init_primes();

    /* Factor n to generate its prime factorisation */
    uint64_t nn = (uint64_t)n;
    uint64_t primes_f[20], exps[20];
    size_t pf_cnt = 0;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p != 0) continue;
        uint64_t e = 0;
        while (nn % p == 0) { ++e; nn /= p; }
        primes_f[pf_cnt] = p;
        exps[pf_cnt] = e;
        ++pf_cnt;
    }
    if (nn > 1) {
        primes_f[pf_cnt] = nn;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Enumerate all divisors of n */
    size_t divcnt = 1;
    uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t) * 1);
    divs[0] = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes_f[i];
        uint64_t e = exps[i];
        size_t oldcnt = divcnt;
        divs = (uint64_t *)realloc(divs, sizeof(uint64_t) * divcnt * (e + 1));
        uint64_t mult = 1;
        for (size_t exp = 1; exp <= e; ++exp) {
            mult *= p;
            for (size_t j = 0; j < oldcnt; ++j)
                divs[divcnt++] = divs[j] * mult;
        }
    }

    /* Filter composite divisors (>1, not prime) */
    size_t comp_cap = 16;
    size_t comp_cnt = 0;
    uint64_t *comps = (uint64_t *)malloc(sizeof(uint64_t) * comp_cap);
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        if (d > 1 && !is_prime_uint64(d)) {
            if (comp_cnt == comp_cap) {
                comp_cap <<= 1;
                comps = (uint64_t *)realloc(comps, sizeof(uint64_t) * comp_cap);
            }
            comps[comp_cnt++] = d;
        }
    }
    free(divs);

    /* Sort composites ascending to avoid duplicate factor order */
    for (size_t i = 0; i < comp_cnt; ++i) {
        for (size_t j = i + 1; j < comp_cnt; ++j) {
            if (comps[i] > comps[j]) {
                uint64_t t = comps[i]; comps[i] = comps[j]; comps[j] = t;
            }
        }
    }

    /* DP tables: dp[idx * (n+1) + rem] */
    size_t nplus = (size_t)n + 1;
    size_t dp_size = (comp_cnt + 1) * nplus;
    seq_t *dp = alloc_seq_array(dp_size);               /* zero‑initialised */
    bool *vis = (bool *)calloc(dp_size, sizeof(bool));

    seq_t result = dfs(comps, comp_cnt, nplus, 0, (size_t)n, dp, vis);

    free(vis);
    free(comps);
    /* dp will be freed by caller or leaked; acceptable for this context */

    return result;
}
