/*
 * A073483  For the n-th squarefree number: the product of all primes greater
 *          than its smallest factor and less than its largest factor and not
 *          dividing it.
 * https://oeis.org/A073483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 46, u64 n <= 75, u128 n <= 131,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product of all primes p such that
 *   smallestPF(s_n) < p < largestPF(s_n)   and   p ∤ s_n,
 * where s_n is the n-th squarefree number.  If no such prime exists,
 * a(n) = 1.
 *
 * The implementation builds a list of primes once, caches the first
 * “n‑th” squarefree numbers on demand, factors each squarefree
 * number by trial division using the cached primes, and then multiplies
 * the qualifying primes.  All arithmetic is performed in seq_t;
 * overflow works modulo the natural width of seq_t, which is
 * acceptable for the builds that support 8‑bit or 16‑bit types.
 */

#include <stdlib.h>      /* malloc, realloc, size_t */
#include "../utils/build_primes.h"

#define PRIME_LIMIT 200000   /* enough for n ≈ 10000 */

static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static int primes_ready = 0;

/* cache of squarefree numbers */
static seq_t *sqfree = NULL;
static size_t sqfree_len = 0;
static idx_t next_check = 1;   /* next integer to test for squarefree */

/* ensure the prime list has been built once */
static void init_primes(void)
{
    if (!primes_ready) {
        build_primes(PRIME_LIMIT, &prime_list, &prime_cnt);
        primes_ready = 1;
    }
}

/* simple factorisation; returns number of distinct prime factors in *cnt,
   stores the primes in pf[0..cnt-1] and their exponents in exp[0..cnt-1].
   All exponents are stored exactly, even if >1. */
static void factor_simple(idx_t n, idx_t *pf, int *exp, int *cnt)
{
    int c = 0;
    idx_t x = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= x; ++i) {
        idx_t p = prime_list[i];
        if (x % p == 0) {
            pf[c] = p;
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            exp[c] = e;
            ++c;
        }
    }
    if (x > 1) {           /* remaining prime factor */
        pf[c] = x;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* test whether m is squarefree; also returns factorisation to avoid
   duplicated work during cache population. */
static int is_squarefree(idx_t m, idx_t *pf, int *exp, int *cnt)
{
    factor_simple(m, pf, exp, cnt);
    for (int i = 0; i < *cnt; ++i)
        if (exp[i] > 1)
            return 0;
    return 1;
}

/* append a new squarefree number to the cache */
static void append_sqfree(seq_t val)
{
    if (sqfree_len == 0) {
        sqfree = (seq_t *)malloc(32 * sizeof(seq_t));
    } else if (sqfree_len % 32 == 0) {
        sqfree = (seq_t *)realloc(sqfree,
                                  (sqfree_len + 32) * sizeof(seq_t));
    }
    sqfree[sqfree_len++] = val;
}

/* ensure the cache contains at least n squarefree numbers */
static void ensure_sqfree(idx_t n)
{
    idx_t pf[32];
    int exp[32];
    int cnt;
    while (sqfree_len < n) {
        if (is_squarefree(next_check, pf, exp, &cnt))
            append_sqfree((seq_t)next_check);
        ++next_check;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 for safety */

    init_primes();
    ensure_sqfree(n);

    seq_t s = sqfree[n - 1];

    /* factor s to get its prime divisors (they are all exponent 1) */
    idx_t pf[32];
    int exp[32];
    int cnt;
    factor_simple((idx_t)s, pf, exp, &cnt);

    if (cnt <= 1)
        return 1;          /* only one or no prime factor → product is 1 */

    idx_t p_min = pf[0];
    idx_t p_max = pf[cnt - 1];

    /* find prime range (p_min, p_max) */
    size_t i = 0;
    while (i < prime_cnt && prime_list[i] <= p_min) ++i;
    size_t j = i;
    while (j < prime_cnt && prime_list[j] < p_max) ++j;

    seq_t prod = 1;
    for (size_t k = i; k < j; ++k) {
        idx_t q = prime_list[k];
        if (s % q != 0)
            prod *= q;
    }
    return prod;
}
