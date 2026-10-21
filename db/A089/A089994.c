/*
 * A089994  Number of primes between factors of n-th semiprime.
 * https://oeis.org/A089994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p | smaller_factor(n) < p < larger_factor(n) },
 *            where the n‑th semiprime = smaller_factor(n) * larger_factor(n)
 * Range:     u8 n <= 915, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If the two factors are equal (the semiprime is a square), a(n)=0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Types provided by the build environment */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Local definitions and state                                          */
/* ------------------------------------------------------------------ */
#define LIMIT 200000U  /* sufficient to cover the first ~10000 semiprimes */

static bool initialized = false;
static uint32_t *primes = NULL;          /* list of primes up to LIMIT           */
static size_t prime_cnt = 0;             /* number of primes                     */
static uint32_t *prime_pi = NULL;        /* prime_pi[x] = number of primes <= x  */
static seq_t *values = NULL;             /* a(1)..a(n) values                    */
static size_t values_cnt = 0;            /* number of computed a(n) values       */

struct semiprime_s {
    uint64_t prod; /* product of the two prime factors */
    uint32_t p;    /* smaller prime factor             */
    uint32_t q;    /* larger prime factor              */
};

/* ------------------------------------------------------------------ */
/* Sieve to generate primes and prefix prime counts                   */
/* ------------------------------------------------------------------ */
static void build_primes_and_pi(void)
{
    /* Allocate and initialize the primality flag array */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(LIMIT + 1));
    if (!is_prime) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Eratosthenes sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* First pass: count primes to allocate arrays */
    prime_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    /* Allocate prime list and prime_pi array */
    primes   = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    prime_pi = (uint32_t *)malloc((size_t)(LIMIT + 1) * sizeof(uint32_t));
    if (!primes || !prime_pi) exit(EXIT_FAILURE);

    /* Second pass: fill arrays */
    size_t pi = 0;
    for (uint32_t i = 0; i <= LIMIT; ++i) {
        if (i >= 2 && is_prime[i]) {
            primes[pi++] = i;
        }
        prime_pi[i] = (uint32_t)pi;
    }

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Compare function for qsort of semiprimes                           */
/* ------------------------------------------------------------------ */
static int compare_semiprime(const void *a, const void *b)
{
    const struct semiprime_s *sa = (const struct semiprime_s *)a;
    const struct semiprime_s *sb = (const struct semiprime_s *)b;
    if (sa->prod < sb->prod) return -1;
    if (sa->prod > sb->prod) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Build all semiprimes ≤ LIMIT and compute corresponding a(n) values */
/* ------------------------------------------------------------------ */
static void build_semiprime_values(void)
{
    /* Rough upper bound of semiprime count up to LIMIT */
    size_t est = (size_t)(LIMIT * 3 / 2);
    struct semiprime_s *semis = (struct semiprime_s *)malloc(est * sizeof(struct semiprime_s));
    if (!semis) exit(EXIT_FAILURE);
    size_t semis_cnt = 0;
    size_t semis_cap = est;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > LIMIT) break;
        for (size_t j = i; j < prime_cnt; ++j) {
            uint32_t q = primes[j];
            uint64_t prod = (uint64_t)p * (uint64_t)q;
            if (prod > LIMIT) break;
            if (semis_cnt == semis_cap) {
                semis_cap *= 2;
                struct semiprime_s *tmp = (struct semiprime_s *)realloc(semis,
                    semis_cap * sizeof(struct semiprime_s));
                if (!tmp) { free(semis); exit(EXIT_FAILURE); }
                semis = tmp;
            }
            semis[semis_cnt].prod = prod;
            semis[semis_cnt].p = p;
            semis[semis_cnt].q = q;
            ++semis_cnt;
        }
    }

    /* Sort semiprimes by product to get the sequence order */
    qsort(semis, semis_cnt, sizeof(struct semiprime_s), compare_semiprime);

    /* Allocate array for a(n) values */
    values = (seq_t *)malloc(semis_cnt * sizeof(seq_t));
    if (!values) { free(semis); exit(EXIT_FAILURE); }

    /* Compute a(n) for each semiprime */
    for (size_t i = 0; i < semis_cnt; ++i) {
        uint32_t p = semis[i].p;
        uint32_t q = semis[i].q;
        if (p == q) {
            values[i] = (seq_t)0;
        } else {
            /* primes strictly between p and q */
            seq_t v = (seq_t)(prime_pi[q - 1] - prime_pi[p]);
            values[i] = v;
        }
    }
    values_cnt = semis_cnt;
    free(semis);
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_primes_and_pi();
        build_semiprime_values();
        initialized = true;
    }
    if (n == 0 || n > values_cnt) return (seq_t)0;
    return values[(size_t)n - 1];
}
