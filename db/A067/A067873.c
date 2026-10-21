/*
 * A067873  Numbers x satisfying x^2 - D*y^2 = 1 for more than one value of D
 *          distinct from x^2 - 1.
 * https://oeis.org/A067873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th integer x (x ≥ 7) for which x²−1 can be
 * expressed as D·y² in at least two distinct ways; i.e. there exist
 * at least two distinct D for which x² − D·y² = 1 has a nontrivial integer
 * solution.  The sequence begins 7, 9, 15, 17, 19, 23, 25, 26, ….
 * The implementation searches x starting at 7 and tests whether (x–1)(x+1)
 * has at least two nontrivial square factors, which is equivalent to
 * requiring more than two divisors of the form y² dividing x²–1.
 */

#include <stdint.h>
#include <stdlib.h>

/* maximum value of x we are ready to test; increases if needed */
#define MAX_X 2000000U

static uint32_t spf[MAX_X + 1];     /* smallest prime factor table */
static uint32_t primes[MAX_X / 10]; /* list of primes up to MAX_X */
static size_t   prime_cnt = 0;

/* Linear sieve for smallest prime factors */
static void init_sieve(void)
{
    size_t i, j;
    for (i = 2; i <= MAX_X; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (j = 0; j < prime_cnt && primes[j] <= spf[i] &&
                 (uint64_t)i * primes[j] <= MAX_X; ++j)
            spf[i * primes[j]] = primes[j];
    }
}

/* factor n (≤ MAX_X) into pf[ ], exp[ ], and return count in *cnt. */
static void add_factor(uint32_t n, uint32_t *pf, uint32_t *exp, size_t *cnt)
{
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t c = 0;
        while (n % p == 0) {
            n /= p;
            ++c;
        }
        size_t k;
        for (k = 0; k < *cnt; ++k) {
            if (pf[k] == p) {
                exp[k] += c;
                goto next;
            }
        }
        /* new prime factor */
        pf[*cnt]   = p;
        exp[*cnt]  = c;
        ++*cnt;
        next:
        ;
    }
}

/* test whether a given integer x is in the sequence */
static int is_special(uint32_t x)
{
    uint32_t pf[32];
    uint32_t exp[32];
    size_t   cnt = 0;

    add_factor(x - 1U, pf, exp, &cnt);
    add_factor(x + 1U, pf, exp, &cnt);

    /* count the number of possible square divisors (y²) > 1 */
    uint32_t prod = 1U;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= (exp[i] / 2U + 1U);
        if (prod >= 3U)         /* already at least two distinct Ds */
            break;
    }
    return prod >= 3U;
}

/* memoized cache of computed terms */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0;
static size_t cache_cap  = 0;
static uint32_t next_val = 7U;   /* next candidate x to test */

/* sequence function */
seq_t a(idx_t n)
{
    static int once = 0;
    if (!once) {
        init_sieve();
        once = 1;
    }

    /* grow cache until it contains at least n terms */
    while (cache_sz < (size_t)n) {
        if (is_special(next_val)) {
            if (cache_sz == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = (seq_t)next_val;
        }
        ++next_val;
    }

    return cache[n - 1];
}
