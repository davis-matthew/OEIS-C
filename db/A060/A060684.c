/*
 * A060684  Smallest difference between consecutive divisors (ordered by size)
 *          of 2n+1.
 * https://oeis.org/A060684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest difference between consecutive divisors of 2n+1
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"

/* Static cache of primes up to the current maximum limit. */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static idx_t    prime_limit   = 0;

/* Ensure that prime_list contains all primes <= limit. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Recursive generation of all divisors given factorisation. */
static void
gen_divs(size_t idx, uint64_t cur,
         const uint64_t *pr, const int *exp,
         size_t fc, uint64_t *out, size_t *pos)
{
    if (idx == fc) {
        out[*pos] = cur;
        (*pos)++;
        return;
    }
    uint64_t p = pr[idx];
    int     e = exp[idx];
    uint64_t v = cur;
    for (int i = 0; i <= e; ++i) {
        gen_divs(idx + 1, v, pr, exp, fc, out, pos);
        v *= p;
    }
}

/* Comparator for qsort on uint64_t values. */
static int
cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y)
        return -1;
    if (x > y)
        return 1;
    return 0;
}

/* Main sequence generator. */
seq_t
a(idx_t n)
{
    uint64_t m = (uint64_t)n * 2ULL + 1ULL;      /* 2n+1 */

    /* Find primes up to sqrt(m). */
    idx_t sqrt_m = int_sqrt(m);
    ensure_primes(sqrt_m);

    /* Factor m. */
    uint64_t  fp[32];
    int       fe[32];
    size_t    fc = 0;
    uint64_t  t = m;

    for (size_t i = 0; i < prime_count && prime_list[i] <= sqrt_m; ++i) {
        uint64_t p = prime_list[i];
        if (t % p != 0)
            continue;
        int e = 0;
        while (t % p == 0) {
            t /= p;
            e++;
        }
        fp[fc] = p;
        fe[fc] = e;
        fc++;
    }
    if (t > 1) {               /* Remaining prime factor. */
        fp[fc] = t;
        fe[fc] = 1;
        fc++;
    }

    /* Compute number of divisors. */
    size_t dcnt = 1;
    for (size_t i = 0; i < fc; ++i)
        dcnt *= (size_t)(fe[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(dcnt * sizeof(uint64_t));
    if (!divs)
        return (seq_t)0; /* allocation failure – unlikely for given limits */

    size_t pos = 0;
    gen_divs(0, 1ULL, fp, fe, fc, divs, &pos);

    /* Sort divisors to compute gaps. */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* Find minimal difference. */
    uint64_t min_diff = UINT64_MAX;
    for (size_t i = 0; i + 1 < dcnt; ++i) {
        uint64_t diff = divs[i + 1] - divs[i];
        if (diff < min_diff)
            min_diff = diff;
    }

    free(divs);
    return (seq_t)min_diff;
}
