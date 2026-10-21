/*
 * A050774  Iterated procedure 'composite k added to sum of its prime factors
 *          reaches a prime' yields 7 skipped primes.
 * https://oeis.org/A050774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A050774
 *  Formula:  a(n) is the n‑th composite integer k such that the iterated
 *  procedure k → k + sum_of_prime_factors(k) reaches a prime p and the
 *  number of primes strictly between k and p equals 7.
 *
 *  Implementation details
 *  – Sieve of Eratosthenes to find all primes ≤ LIMIT.
 *  – Smallest prime factor array for fast factorisation.
 *  – Prefix sum of prime counts to count the primes between two values.
 *  – Linear search for terms, caching the results.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#if !defined(idx_t)
typedef uint64_t idx_t;               /* supplied by the build system */
#endif
/* seq_t is also supplied by the build system – no definition needed here */

/* ------------------------------------------------------------ */
/*  Parameters and static data                                 */
/* ------------------------------------------------------------ */
#define LIMIT      4000000          /* sieve range – enough for n ≤ 10000 */
#define MAX_TERMS  20000            /* capacity for cached terms  */
#define MAX_PRIMES 300000           /* maximum number of primes in sieve */

static bool     is_prime[LIMIT + 1];
static int      spf[LIMIT + 1];     /* smallest prime factor */
static uint32_t prime_prefix[LIMIT + 1];

static seq_t terms[MAX_TERMS];
static idx_t term_count   = 0;      /* how many terms have been found   */
static idx_t next_candidate = 4;    /* next composite to test           */
static bool  prepared   = false;    /* sieve initialised? */

static int primes[MAX_PRIMES];
static int primes_cnt  = 0;

/* ------------------------------------------------------------ */
/*  Sieve and precomputation                                  */
/* ------------------------------------------------------------ */
static void prepare(void)
{
    if (prepared) return;

    /* ----- 1. Eratosthenes sieve ----------------------------------- */
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* ----- 2. Collect primes ------------------------------------------ */
    for (int i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_cnt < MAX_PRIMES) {
                primes[primes_cnt++] = i;
            }
        }
    }

    /* ----- 3. Smallest prime factor array ------------------------------ */
    for (int i = 0; i <= LIMIT; ++i) spf[i] = i;   /* (primes stay as themselves) */
    for (int idx = 0; idx < primes_cnt; ++idx) {
        int p = primes[idx];
        for (int j = p * 2; j <= LIMIT; j += p) {
            if (spf[j] == j) {
                spf[j] = p;
            }
        }
    }

    /* ----- 4. Prefix sum of prime counts -------------------------------- */
    prime_prefix[0] = 0;
    for (int i = 1; i <= LIMIT; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + (is_prime[i] ? 1u : 0u);
    }

    prepared = true;
}

/* ------------------------------------------------------------ */
/*  Helper functions                                           */
/* ------------------------------------------------------------ */
/* Sum of prime factors of x (with multiplicity) – x must be ≤ LIMIT */
static int sum_prime_factors(int x)
{
    int sum = 0;
    while (x > 1) {
        int p = spf[x];
        sum += p;
        x /= p;
    }
    return sum;
}

/* Final prime reached from composite k by the iterative process */
static int final_prime_from(int k)
{
    int cur = k;
    while (!is_prime[cur]) {
        cur += sum_prime_factors(cur);
    }
    return cur;   /* cur ≤ LIMIT for the range we are interested in */
}

/* ------------------------------------------------------------ */
/*  Public function                                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for 0, but safety */

    prepare();

    /* Generate terms until we have at least n of them */
    while (term_count < n) {
        while (next_candidate <= LIMIT && is_prime[next_candidate]) {
            ++next_candidate;            /* skip primes */
        }
        if (next_candidate > LIMIT) {     /* out of pre‑computed sieve */
            return (seq_t)0;
        }

        /* composite = next_candidate */
        int fin = final_prime_from((int)next_candidate);
        if (fin > LIMIT) {                 /* safety check – shouldn't happen */
            return (seq_t)0;
        }

        uint32_t primes_between =
            prime_prefix[fin - 1] - prime_prefix[(int)next_candidate];
        if (primes_between == 7u) {
            /* store term if we have space */
            if (term_count < MAX_TERMS) {
                terms[term_count++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
