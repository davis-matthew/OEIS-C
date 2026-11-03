/*
 * A085073  Smallest k such that n+k and n*k have the same prime signature, or
 *          0 if no such number exists.
 * https://oeis.org/A085073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that prime signature of n*k equals the
 *            prime signature of n+k. Return 0 if no such k exists. The
 *            signature is the multiset of exponents in the prime
 *            factorisation, regardless of which primes occur
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 85 terms through n = 2726 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

/* combine exponents of n and k into val[], returning number of exponents.
   n's and k's primes and exponents are given. */
static size_t combine_exponents(const uint64_t pr_n[],
                                const uint64_t exp_n[], size_t cn,
                                const uint64_t pr_k[],
                                const uint64_t exp_k[], size_t ck,
                                uint64_t val[])
{
    size_t cnt = 0;
    /* copy n's exponents */
    for (size_t i = 0; i < cn; ++i)
        val[cnt++] = exp_n[i];

    /* add k's exponents, merging with existing primes */
    for (size_t i = 0; i < ck; ++i) {
        uint64_t p = pr_k[i];
        bool merged = false;
        /* search if prime appears among n's primes */
        for (size_t j = 0; j < cn; ++j) {
            if (pr_n[j] == p) {
                val[j] += exp_k[i];
                merged = true;
                break;
            }
        }
        if (!merged)
            val[cnt++] = exp_k[i];
    }
    return cnt;
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* factor n once */
    uint64_t pr_n[64], exp_n[64];
    size_t cn;
    _factor_u64((uint64_t)n, pr_n, exp_n, &cn);

    /* buffers for factorising k and n+k */
    uint64_t pr_k[64], exp_k[64];
    uint64_t pr_sum[64], exp_sum[64];
    uint64_t exps_prod[128];

    /* search for smallest k */
    for (uint64_t k = 1;; ++k) {
        uint64_t np = (uint64_t)n + k;

        /* factor n+k */
        size_t cs;
        _factor_u64(np, pr_sum, exp_sum, &cs);

        /* factor k */
        size_t ck;
        _factor_u64(k, pr_k, exp_k, &ck);

        /* exponent multiset of n*k */
        size_t cp = combine_exponents(pr_n, exp_n, cn, pr_k, exp_k, ck,
                                      exps_prod);

        /* sort both exponent lists */
        qsort(exp_sum, cs, sizeof(uint64_t), cmp_uint64);
        qsort(exps_prod, cp, sizeof(uint64_t), cmp_uint64);

        /* compare */
        if (cp == cs) {
            bool equal = true;
            for (size_t i = 0; i < cp; ++i) {
                if (exps_prod[i] != exp_sum[i]) {
                    equal = false;
                    break;
                }
            }
            if (equal)
                return (seq_t)k;
        }

        /* safeguard against infinite loops */
        /* the sequence data suggests k rarely exceeds a few million for n≤10000 */
        if (k > 10000000ULL)
            break;
    }

    return (seq_t)0;
}
