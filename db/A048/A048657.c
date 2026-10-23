/*
 * A048657  Number of non-unitary divisors of n!.
 * https://oeis.org/A048657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n!) - 2^(A000720(n))
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 49, u64 n <= 118,
 *            u128 n <= 282, big any n
 * Helpers:   build_primes_upto, build_spf, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of non‑unitary divisors of n! */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/pow2.h"

static idx_t max_comp = 0;              /* highest n processed so far          */
static uint64_t *primes = NULL;         /* list of primes up to max_comp       */
static size_t primes_cnt = 0;           /* number of primes in that list      */
static idx_t *spf = NULL;              /* smallest prime factor for each i   */
static idx_t spf_size = 0;              /* actual length of spf[] (max_comp+1)*/
static seq_t *d_cache = NULL;           /* d_cache[i] = d(i!)                 */
static size_t d_cache_size = 0;         /* current allocated size of d_cache[]*/
static uint32_t *prime_exps = NULL;     /* current exponent of each prime in i!*/
static size_t prime_exps_size = 0;      /* allocated length of prime_exps[]   */

/* Binary search to find the index of prime p in primes[] */
static size_t prime_index(uint64_t p) {
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p)
            return mid;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return primes_cnt; /* p must exist when this is called */
}

/* Ensure all auxiliary arrays are at least up to n */
static void ensure_extended(idx_t n) {
    if (n <= max_comp)
        return;

    /* ---- build prime list up to n ---- */
    uint64_t *new_primes;
    size_t new_cnt;
    build_primes_upto(n, &new_primes, &new_cnt);
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;

    /* ---- build smallest prime factor array up to n ---- */
    idx_t *new_spf;
    build_spf(n, &new_spf);
    free(spf);
    spf = new_spf;
    spf_size = n + 1;

    /* ---- resize prime_exps if necessary ---- */
    if (prime_exps_size < primes_cnt) {
        uint32_t *tmp = (uint32_t *)realloc(prime_exps,
                                            primes_cnt * sizeof(uint32_t));
        if (!tmp)
            abort();
        prime_exps = tmp;
        for (size_t i = prime_exps_size; i < primes_cnt; ++i)
            prime_exps[i] = 0;
        prime_exps_size = primes_cnt;
    }

    /* ---- resize d_cache if necessary ---- */
    if (d_cache_size <= (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(d_cache,
                                      (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        d_cache = tmp;
        d_cache_size = n + 1;
    }
    if (max_comp == 0 && d_cache_size > 0)
        d_cache[0] = 1; /* d(0!) = 1 */

    /* ---- compute d(i!) for new i ---- */
    seq_t d_now = d_cache[max_comp > 0 ? max_comp : 0];
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        idx_t m = i;
        while (m > 1) {
            uint64_t p = spf[m];
            uint32_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            size_t idx = prime_index(p);
            uint32_t old_exp = prime_exps[idx];
            uint32_t new_exp = old_exp + cnt;
            prime_exps[idx] = new_exp;

            /* Update d_now avoiding overflow:
               d_now = d_now / (old_exp+1) * (new_exp+1); */
            seq_t tmp = d_now / (seq_t)(old_exp + 1);
            d_now = tmp * (seq_t)(new_exp + 1);
        }
        d_cache[i] = d_now;
    }

    max_comp = n;
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined for n < 1, function used only for n>=1 */
    ensure_extended(n);

    seq_t d_n     = d_cache[n];
    seq_t pow2_pi = pow2((idx_t)primes_cnt);   /* 2^(π(n))   */
    return d_n - pow2_pi;
}
