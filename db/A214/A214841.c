/*
 * A214841  Number of ways to write n=p+q/2, where p and q are practical
 *          numbers smaller than n.
 * https://oeis.org/A214841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4619, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9927 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of ways to write n = p + q/2 with practical p,q < n
   (OEIS A214841). The function is memoised and works for any order of
   calls.  All allocations are cast to the proper pointer type for C++.
   The algorithm first generates all practical numbers up to a given
   bound and then counts, for each n, the pairs (p,q) that satisfy the
   required condition.  The search space is O(n) per n.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether n is a practical number using the known characterization. */
static bool is_practical_number(idx_t n, idx_t *prime_list, size_t prime_count)
{
    if (n == 1)
        return true;
    if ((n & 1UL) != 0)
        return false;   /* odd numbers >1 are not practical */

    idx_t temp = n;
    idx_t sigma_prefix = 1;  /* σ(product of processed prime powers) */
    idx_t last_prime = 0;    /* last prime processed */

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
        idx_t p = prime_list[i];
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (last_prime == 0 && p != 2)
                return false;   /* first prime must be 2 */
            if (last_prime != 0 && p > sigma_prefix + 1)
                return false;   /* p_i <= σ(prev)+1 fails */

            /* σ(p^exp) = 1 + p + ... + p^exp */
            idx_t sigma_pe = 1;
            idx_t p_pow    = 1;
            for (idx_t k = 0; k < exp; ++k) {
                p_pow *= p;
                sigma_pe += p_pow;
            }
            sigma_prefix *= sigma_pe;
            last_prime = p;
        }
    }

    if (temp > 1) {      /* remaining prime factor (exponent 1) */
        idx_t p = temp;
        if (last_prime == 0 && p != 2)
            return false;
        if (last_prime != 0 && p > sigma_prefix + 1)
            return false;
        idx_t sigma_pe = 1 + p;   /* exponent 1 */
        sigma_prefix *= sigma_pe;
    }

    return true;
}

/* Integer square root (floor).  Works for idx_t (uint64_t). */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* Generate all primes <= limit.  The caller must free the array. */
static void generate_primes_upto(idx_t limit, idx_t **primes, size_t *cnt)
{
    size_t cap = 0;
    *primes = NULL;
    *cnt    = 0;
    bool *mark = (bool*)calloc(limit + 1, sizeof(bool));
    for (idx_t p = 2; p <= limit; ++p) {
        if (!mark[p]) {
            if (*cnt == cap) {
                cap = (cap == 0) ? 16 : cap * 2;
                *primes = (idx_t*)realloc(*primes, cap * sizeof(idx_t));
            }
            (*primes)[(*cnt)++] = p;
            if (p * p <= limit) {
                for (idx_t j = p * p; j <= limit; j += p)
                    mark[j] = true;
            }
        }
    }
    free(mark);
}

/* Cached data for memoisation. */
static bool *practical_flags = NULL;      /* practical[n] == true/false */
static size_t practical_cap  = 0;
static idx_t  practical_cnt  = 0;
static idx_t *practicals     = NULL;      /* list of practical numbers */

static seq_t *dp     = NULL;              /* dp[n] = a(n) */
static idx_t  dp_cap = 0;
static idx_t  dp_len = 0;                 /* largest n already computed */

static idx_t precomp_limit = 0;           /* highest n for which a(n) known */

/* Pre‑compute all needed information at least up to 'wanted'. */
static void extend_precomp(idx_t wanted)
{
    if (wanted <= precomp_limit)
        return;

    /* Resize practical_flags array if needed. */
    if (wanted + 1 > precomp_limit + 1) {
        bool *tmp = (bool*)realloc(practical_flags, (wanted + 1) * sizeof(bool));
        if (!tmp) exit(1);
        practical_flags = tmp;
        for (idx_t i = precomp_limit + 1; i <= wanted; ++i)
            practical_flags[i] = false;
    }

    /* Generate primes up to sqrt(wanted) for factorising. */
    idx_t limit = int_sqrt(wanted);
    idx_t *prime_list = NULL;
    size_t prime_count = 0;
    generate_primes_upto(limit, &prime_list, &prime_count);

    /* Find all practical numbers in the new range. */
    for (idx_t n = precomp_limit + 1; n <= wanted; ++n) {
        bool ok = is_practical_number(n, prime_list, prime_count);
        practical_flags[n] = ok;
        if (ok) {
            if (practical_cnt == practical_cap) {
                practical_cap = practical_cap ? practical_cap * 2 : 16;
                practicals   = (idx_t*)realloc(practicals, practical_cap * sizeof(idx_t));
                if (!practicals) exit(1);
            }
            practicals[practical_cnt++] = n;
        }
    }
    free(prime_list);

    /* Compute a(n) for the new n range (starting after dp_len). */
    for (idx_t n = dp_len + 1; n <= wanted; ++n) {
        seq_t cnt = 0;
        idx_t half = n / 2;
        for (size_t i = 0; i < practical_cnt; ++i) {
            idx_t p = practicals[i];
            if (p <= half || p >= n)
                continue;
            idx_t q = 2 * (n - p);
            if (q < n && practical_flags[q])
                ++cnt;
        }
        if (dp_len == dp_cap) {
            dp_cap = dp_cap ? dp_cap * 2 : 16;
            dp = mem_resize(seq_t, dp, (dp_cap + 1));
            if (!dp) exit(1);
        }
        dp[n] = cnt;
        ++dp_len;
    }

    precomp_limit = wanted;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* offset is 1 */
    /* Ensure we have enough precomputation for q = 2(n-p). */
    extend_precomp(n * 2);
    return dp[n];
}
