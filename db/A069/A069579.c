/*
 * A069579  Numbers k such that phi(core(k)) = core(phi(k)) where core(k) is
 *          the squarefree part of k (the smallest integer such that k*core(k)
 *          is a square).
 * https://oeis.org/A069579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th positive integer k such that φ(core(k)) =
 *            core(φ(k)) where core(k) is the squarefree part of k
 * Range:     u8 n <= 53, u16 n <= 4416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t types are supplied by the build system. */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* --------------------------------------------------------------------- */
/* A simple linear‑time sieve to obtain smallest prime factors (spf) and
   a list of primes up to a fixed limit.  The values are only needed for
   integers that comfortably fit in 32 bits, which is sufficient for
   the required index range (up to about 10 000). */

#define SIEVE_LIMIT 2000000   /* enough for k values up to ~3·10⁵ */

static uint32_t *spf      = NULL;   /* spf[n] = smallest prime factor of n */
static uint32_t *primes   = NULL;   /* list of primes */
static size_t    primes_cnt = 0;
static bool      sieve_done  = false;

static void init_sieve(void)
{
    if (sieve_done) return;

    spf = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);

    size_t prime_cap = SIEVE_LIMIT / 10 + 10;
    primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) exit(1);

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j)
        {
            uint32_t p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > SIEVE_LIMIT) break;
            spf[i * p] = p;
        }
    }
    sieve_done = true;
}

/* --------------------------------------------------------------------- */
/* Core of a small integer: product of distinct prime factors (eliminate
   even exponents).  The result fits in uint64_t; cast to seq_t. */

static seq_t core_of_uint32(uint32_t n)
{
    seq_t res = 1;
    while (n > 1)
    {
        uint32_t p = spf[n];
        uint8_t cnt = 0;
        while (n % p == 0)
        {
            n /= p;
            ++cnt;
        }
        if (cnt & 1)
            res *= (seq_t)p;
    }
    return res;
}

/* Chronological φ of a small integer, using spf. */

static seq_t phi_of_uint32(uint32_t n)
{
    seq_t res = n;
    while (n > 1)
    {
        uint32_t p = spf[n];
        while (n % p == 0) n /= p;
        res = res / p * (p - 1);
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* Sequence construction – a cache of found terms.  For each new integer
   k we test the equality φ(core(k)) == core(φ(k)).  When the cache
   contains n terms we can answer any query for index n immediately. */

static seq_t *terms    = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t   current_k = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety – not part of the sequence */

    init_sieve();

    /* Grow cache until it holds at least n terms. */
    while (terms_len < (size_t)n)
    {
        ++current_k;
        uint32_t kk = (uint32_t)current_k;           /* kk fits in 32‑bit */

        seq_t core_k  = core_of_uint32(kk);
        seq_t phi_k   = phi_of_uint32(kk);

        seq_t phi_core_k = phi_of_uint32((uint32_t)core_k);
        seq_t core_phi_k = core_of_uint32((uint32_t)phi_k);

        if (phi_core_k == core_phi_k)
        {
            /* store the term */
            if (terms_len == terms_cap)
            {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) exit(1);
            }
            terms[terms_len++] = (seq_t)current_k;
        }
    }

    return terms[n - 1];
}
