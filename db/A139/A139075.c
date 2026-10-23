/*
 * A139075  Primes p arising in A139074.
 * https://oeis.org/A139075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A139075: a(n) = the smallest prime q such that
 *  q = (p! + n)/n for some prime p.  The function computes the
 *  minimal prime q for every n >= 1 by iterating over primes p
 *  in ascending order and checking that n divides p! and that the
 *  resulting q is prime.  The factorial is built incrementally
 *  using big‐integer arithmetic, and the divisor checks are
 *  done by comparing prime exponents via Legendre’s formula. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* primality test for arbitrarily large integers */
#include "../utils/is_prime_big.h"   /*  is_prime_big(seq_t n) */

/* ------------------------------------------------------------------ */
/*  Global state  */
static bool          init_done   = false;
static idx_t        *primes      = NULL;
static size_t        prime_cnt   = 0;

/* ------------------------------------------------------------------ */
/*  Sieve of Eratosthenes up to LIMIT, store primes in array */
static void init(void)
{
    const idx_t LIMIT = 200000;        /* sufficient for n <= 10000 */

    /* allocate sieve array */
    bool *is_comp = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_comp) { init_done = true; return; }

    for (idx_t i = 0; i <= LIMIT; ++i) is_comp[i] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* temporary storage */
    size_t tmp_cap = LIMIT / 10 + 10;
    primes = (idx_t *)malloc(tmp_cap * sizeof(idx_t));
    if (!primes) { free(is_comp); init_done = true; return; }

    prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) {
            if (prime_cnt == tmp_cap) {
                tmp_cap *= 2;
                primes = (idx_t *)realloc(primes, tmp_cap * sizeof(idx_t));
            }
            primes[prime_cnt++] = i;
        }

    free(is_comp);
    init_done = true;
}

/* ------------------------------------------------------------------ */
/*  Factorise n into small arrays (n <= 10000).  Caller must provide
 *  arrays with at least 8 slots.   Result in factors[0..cnt-1],
 *  exponents[0..cnt-1], and cnt. */
static void factorize(idx_t n, idx_t *factors, idx_t *exps, size_t *cnt)
{
    *cnt = 0;
    idx_t rem = n;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= rem; ++i) {
        idx_t p = primes[i];
        if (rem % p == 0) {
            idx_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            factors[(*cnt)]   = p;
            exps    [(*cnt)]  = e;
            ++(*cnt);
        }
    }
    if (rem > 1) {
        factors[(*cnt)]   = rem;
        exps    [(*cnt)]  = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/*  Legendre's formula: exponent of prime pf in k!   */
static idx_t exp_in_fact(idx_t pf, idx_t k)
{
    idx_t sum = 0;
    idx_t div = pf;
    while (div <= k) {
        sum += k / div;
        if (div > k / pf) break;
        div *= pf;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Main sequence computation function */
seq_t a(idx_t n)
{
    if (!init_done) init();
    if (n == 0) return 0;        /* undefined for n=0, return 0 */

    /* factorise n */
    idx_t factors[8], exps[8];
    size_t nfactors;
    factorize(n, factors, exps, &nfactors);

    /* iterate over primes, compute factorial incrementally */
    seq_t fact = (seq_t)1;
    size_t prime_idx = 0;        /* index into primes array */

    for (idx_t i = 2; ; ++i) {
        fact = fact * (seq_t)i;          /* fact = i! */

        if (i == primes[prime_idx]) {    /* i is prime */
            /* 1) check that n divides i! */
            bool ok = true;
            for (size_t j = 0; j < nfactors; ++j) {
                if (exp_in_fact(factors[j], i) < exps[j]) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                /* 2) compute q = (i! + n)/n  */
                seq_t q = (fact + (seq_t)n) / (seq_t)n;
                if (is_prime_big(q)) return q;
            }
            ++prime_idx;
        }
    }
    /* unreachable, but suppress compiler warning */
    return 0;
}
