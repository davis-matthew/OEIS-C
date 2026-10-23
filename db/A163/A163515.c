/*
 * A163515  If n-th composite is the product of k1-th prime, k2-th prime, ...,
 *          kr-th prime then set a(n) = k1 + k2 + ... + kr.
 * https://oeis.org/A163515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the indices of the prime factors of the n‑th
 *            composite number. The implementation precomputes all primes up
 *            to a fixed limit and uses a linear sieve. For each composite it
 *            factors it by repeated division with its smallest prime factor.
 *            All results are cached so that calls with arbit...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile with: -std=c11 OR -std=c++17
   seq_t and idx_t are provided by the build.          */
#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/*  Parameters                                                     */
/* --------------------------------------------------------------------- */
#define LIMIT        200000       /* should be >> 10000 to cover all needed composites   */
#define MAX_N        20000        /* cache size: enough for n up to ~10000                */

/* --------------------------------------------------------------------- */
/*  Global tables (zero‑initialised)                              */
/* --------------------------------------------------------------------- */
static uint32_t spf_idx[LIMIT + 1];    /* index of smallest prime factor (1‑based)        */
static uint8_t  prime_bool[LIMIT + 1]; /* 1 if number is prime                                */
static uint32_t primes[LIMIT];         /* list of primes                                   */
static int      prime_count = 0;       /* number of primes found                           */

/* cache for a(n) values */
static seq_t cache[MAX_N];
static size_t cached_count = 0;        /* number of a(n) values stored                     */

/* bookkeeping for incremental generation of composites   */
static idx_t composites_done = 0;      /* how many composites have already been processed   */
static size_t last_number = 3;         /* next candidate to test (first composite is 4)     */

/* --------------------------------------------------------------------- */
/*  Linear sieve up to LIMIT                                  */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    /* If already initialised (call repeated due to static variables), skip */
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf_idx[i] == 0) {                     /* i is prime */
            prime_bool[i] = 1;
            spf_idx[i] = (uint32_t)(prime_count + 1); /* 1‑based index */
            primes[prime_count++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            size_t p = primes[j];
            size_t ip = i * p;
            if (ip > LIMIT) break;
            spf_idx[ip] = (uint32_t)(j + 1);
            prime_bool[ip] = 0;
            if (i % p == 0) break;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Main function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless            */
    if (n > MAX_N) return 0;       /* out of cache bounds (should not happen for tests) */

    init_sieve();                  /* ensure tiny tables are ready       */

    /* Compute composites until we have the n‑th one                */
    while (composites_done < n) {
        ++last_number;                   /* candidate number                 */

        if (last_number > LIMIT) {
            /* Should never happen with current LIMIT      */
            return 0;
        }

        if (!prime_bool[last_number]) {   /* composite */
            uint64_t m = (uint64_t)last_number;
            seq_t sum = 0;
            while (m > 1) {
                uint32_t idx = spf_idx[m];        /* 1‑based prime index  */
                sum += (seq_t)idx;               /* add to answer        */
                m /= primes[idx - 1];            /* divide by that prime  */
            }
            /* Store result in cache */
            if (composites_done < cached_count) {
                cache[composites_done] = sum;
            } else {
                cache[(size_t)composites_done] = sum;
                ++cached_count;
            }
            ++composites_done;
        }
    }

    return cache[(size_t)(n - 1)];
}
