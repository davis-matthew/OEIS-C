/*
 * A104485  Primes p = p(k) such that prime(k) + 2 and prime(k+1) + 2 are both
 *          semiprimes.
 * https://oeis.org/A104485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(k) where prime(k)+2 and prime(k+1)+2 are both
 *            semiprimes
 * Range:     u8 n <= 9, u16 n <= 697, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static tables and helpers */
static uint32_t *spf      = NULL;   /* smallest prime factor table */
static uint32_t *primes   = NULL;   /* list of primes up to LIMIT */
static seq_t   *terms    = NULL;   /* sequence values (primes satisfying the condition) */
static size_t  terms_len = 0, terms_cap = 0;

static bool initialized = false;

#define LIMIT 12000000U   /* enough to produce > 10 000 terms */

/* --------- helper : test whether a number is semiprime ----------------- */
static inline bool is_semiprime(uint32_t x)
{
    if (x < 4) return false;
    uint32_t p = spf[x];
    if (p == x) return false;      /* prime */
    uint32_t m = x / p;
    return spf[m] == m;            /* m must be prime */
}

/* --------- init function : build spf, primes and terms ----------------- */
static void init(void)
{
    const uint32_t N = LIMIT + 2U;                 /* cover prime+2 values */
    const size_t size_spf = (size_t)(N + 1);

    spf = (uint32_t *)malloc(size_spf * sizeof(uint32_t));
    for (size_t i = 0; i < size_spf; ++i) spf[i] = 0;

    /* sieve for spf */
    for (uint32_t i = 2; i <= N; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if ((uint64_t)i * i <= N) {
                for (uint64_t j = (uint64_t)i * i; j <= N; j += i)
                    if (!spf[(size_t)j]) spf[(size_t)j] = i;
            }
        }
    }

    /* collect primes up to LIMIT */
    const size_t prime_cap_est = (size_t)(LIMIT / 10);   /* generous */
    primes = (uint32_t *)malloc(prime_cap_est * sizeof(uint32_t));
    size_t prime_cap = prime_cap_est;
    size_t prime_cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == i) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = i;
        }
    }

    /* build the sequence */
    terms_cap = 2000;  /* start capacity */
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    terms_len = 0;

    for (size_t idx = 0; idx + 1 < prime_cnt; ++idx) {
        uint32_t a = primes[idx];
        uint32_t b = primes[idx + 1];
        if (is_semiprime(a + 2U) && is_semiprime(b + 2U)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)a;
        }
    }
}

/* ------------------------------- the requested function ----------------- */
seq_t a(idx_t n)
{
    if (!initialized) { init(); initialized = true; }
    if (n <= 0) return 0;
    if ((size_t)(n - 1) >= terms_len) return 0;  /* out of pre‑computed range */
    return terms[n - 1];
}
