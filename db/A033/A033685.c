/*
 * A033685  Theta series of hexagonal lattice A_2 with respect to deep hole.
 * https://oeis.org/A033685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * sum_{d|n , d mod 9 != 0} sgn(d) where sgn(d) is
 *            defined by the residue of d modulo 9: +1 for residues 1,4,6,7 -1
 *            for residues 2,3,5,8 (residue 0 never appears in the sum)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* sign table indexed by residue mod 9 (0 unused) */
static const int sign_map[9] = {
    0,  /* 0  unused */
    1,  /* 1  +1 */
   -1,  /* 2  -1 */
   -1,  /* 3  -1 */
    1,  /* 4  +1 */
   -1,  /* 5  -1 */
    1,  /* 6  +1 */
    1,  /* 7  +1 */
   -1   /* 8  -1 */
};

/* local cache of sequence values */
static seq_t *cache = NULL;      /* array of a(i) */
static idx_t  cache_nmax = 0;    /* number of elements stored */

/* recursive DFS to enumerate all divisors from prime factorisation */
static void dfs_divisors(const idx_t *pr, const int *ex, int pos, int num,
                         idx_t curr, int *sum)
{
    if (pos == num) {
        if (curr % 9 != 0) {
            int mod = (int)(curr % 9);
            *sum += sign_map[mod];
        }
        return;
    }
    idx_t p = pr[pos];
    idx_t val = 1;
    for (int e = 0; e <= ex[pos]; ++e) {
        dfs_divisors(pr, ex, pos + 1, num, curr * val, sum);
        val *= p;
    }
}

/* compute a(n) for a single n */
static seq_t compute_value(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* factor n into primes */
    idx_t m = n;
    idx_t primes[16];
    int   exps[16];
    int    fac_cnt = 0;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[fac_cnt] = p;
            exps[fac_cnt]  = e;
            ++fac_cnt;
        }
    }
    if (m > 1) {          /* remaining prime factor */
        primes[fac_cnt] = m;
        exps[fac_cnt]  = 1;
        ++fac_cnt;
    }

    /* enumerate all divisors and sum their signs */
    int sum = 0;
    dfs_divisors(primes, exps, 0, fac_cnt, (idx_t)1, &sum);

    /* final result */
    seq_t res = (seq_t)((idx_t)3 * (idx_t)sum);
    return res;
}

/* extend cache up to n */
static void extend_cache(idx_t n)
{
    if (n < cache_nmax) return;          /* already computed */

    if (!cache) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) abort();              /* allocation failure */
        cache[0] = (seq_t)0;
        cache_nmax = 1;
    } else if (n + 1 > cache_nmax) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    idx_t start = (cache_nmax > 1 ? cache_nmax : 1);
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = compute_value(i);
    }
    cache_nmax = n + 1;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
