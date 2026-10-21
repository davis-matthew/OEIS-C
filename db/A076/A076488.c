/*
 * A076488  Nonsquarefree solutions to gcd(sigma(x), phi(x)) =
 *          gcd(sigma(core(x)), Phi(core(x))), i.e., when A009223(x) =
 *          A066086(x) or if A066087(x)=0 and mu(x)=0.
 * https://oeis.org/A076488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer n such that gcd(σ(n),φ(n))
 *            equals gcd(σ(core(n)),φ(core(n))) with μ(n)=0 (i.e., n is not
 *            square‑free). See OEIS A076488
 * Range:     u8 n <= 14, u16 n <= 2308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_seq_t, grow_int_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_seq_t.h"
#include "../utils/grow_int_array.h"

/* internal types (seq_t and idx_t are provided by the build) */
static size_t LIMIT = 2000000;        /* initial search bound */
static int *spf = NULL;               /* smallest prime factor */
static int *mu  = NULL;               /* Möbius function values */
static int  *primes = NULL;           /* list of primes */
static size_t primes_cap = 0;
static size_t primes_cnt = 0;
static size_t processed_upto = 0;     /* last number processed */

/* array holding the sequence terms that have been found */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_cnt = 0;

/* linear sieve up to n, filling spf, mu and primes */
static void init_sieve(size_t n)
{
    if (spf != NULL) free(spf);
    if (mu  != NULL) free(mu);
    if (primes != NULL) free(primes);
    if (terms != NULL) free(terms);
    processed_upto = 0;
    terms_cnt = 0;
    terms_cap = 0;
    terms = NULL;
    spf = (int *)calloc(n + 1, sizeof(int));
    mu  = (int *)calloc(n + 1, sizeof(int));
    primes_cap = n / 10 + 10;
    primes = (int *)calloc(primes_cap, sizeof(int));
    primes_cnt = 0;

    mu[1] = 1;
    for (size_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            if (primes_cnt >= primes_cap) grow_int_array(&primes, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = (size_t)primes[j];
            if (p > (size_t)spf[i] || i * p > n) break;
            spf[i * p] = (int)p;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            } else {
                mu[i * p] = -mu[i];
            }
        }
    }
    LIMIT = n;
}

/* test if number x satisfies the required condition.
   If it does, push its value into the terms array. */
static void process_number(idx_t x)
{
    if (x < 2) return;        /* 1 is not considered */
    seq_t sigma = 1;
    seq_t phi   = 1;
    seq_t rad_sigma = 1;
    seq_t rad_phi   = 1;
    bool sqfree = true;
    idx_t m = x;

    while (m > 1) {
        int p = spf[m];
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* sigma(p^e) = 1 + p + ... + p^e */
        seq_t sum = 1;
        seq_t powp = 1;
        for (int i = 0; i < e; ++i) {
            powp *= (seq_t)p;
            sum += powp;
        }
        sigma *= sum;

        /* phi(p^e) = p^{e-1}*(p-1) */
        seq_t phi_pe;
        if (e == 1) {
            phi_pe = (seq_t)(p - 1);
        } else {
            seq_t mult = 1;
            for (int i = 1; i < e; ++i) mult *= (seq_t)p;
            phi_pe = mult * (seq_t)(p - 1);
        }
        phi *= phi_pe;

        /* for core: exponent 1 */
        rad_sigma *= (seq_t)(p + 1);
        rad_phi   *= (seq_t)(p - 1);

        if (e >= 2) sqfree = false;   /* square factor found */
    }

    if (!sqfree) {
        seq_t g1 = gcd_seq_t(sigma, phi);
        seq_t g2 = gcd_seq_t(rad_sigma, rad_phi);
        if (g1 == g2) {
            if (terms_cnt == terms_cap) {
                size_t new_cap = (terms_cap == 0) ? 2 : (terms_cap << 1);
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)x;
        }
    }
}

/* ensure that terms array contains at least `need` entries */
static void ensure_terms(size_t need)
{
    while (terms_cnt < need) {
        /* if we haven't processed up to LIMIT, continue */
        if (processed_upto < LIMIT) {
            ++processed_upto;
            process_number(processed_upto);
            continue;
        }
        /* not enough terms yet – enlarge the search bound */
        size_t new_limit = LIMIT * 2;
        init_sieve(new_limit);
        processed_upto = 1;   /* start fresh after new sieve */
    }
}

static bool initialized = false;

/* the required function */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve(LIMIT);
        processed_upto = 0;
        initialized = true;
    }
    if (n == 0) return (seq_t)0;    /* undefined, but safe */
    ensure_terms((size_t)n);
    return terms[n - 1];
}
