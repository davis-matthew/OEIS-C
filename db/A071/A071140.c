/*
 * A071140  Numbers n such that sum of distinct primes dividing n is divisible
 *          by largest prime dividing n; n is neither a prime, nor a true
 *          power of prime.
 * https://oeis.org/A071140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = the n-th smallest integer with at least three distinct
 *            prime factors, * for which the sum of its distinct prime
 *            divisors is divisible by its * largest prime divisor. * This is
 *            equivalent to: for the set P of distinct primes dividing x, *
 *            (sum(P) mod max(P) == 0) and |P| ≥ 3
 * Range:     u8 n <= 9, u16 n <= 317, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static int initialized = 0;

/*  Storage for sieve information                              */
static uint32_t *spf = NULL;            /* smallest prime factor                */
static uint32_t *lpf = NULL;            /* largest prime factor                  */
static uint32_t *sumd = NULL;           /* sum of distinct primes                */
static uint8_t  *cntd = NULL;           /* count of distinct primes              */

/*  Storage for the sequence values                              */
static seq_t *seq_terms = NULL;
static size_t seq_size = 0;
static size_t seq_capacity = 0;

/*  Maximum number to sieve.  Chosen large enough to contain at
 *  least 10 000 terms (empirical density suggests ~3 % density).
 */
#define MAX_LIMIT 4000000U

static void compute_sequence(void)
{
    size_t i, j, limit = MAX_LIMIT;
    size_t primes_count = 0;
    uint32_t *primes = (uint32_t*)malloc(limit * sizeof(uint32_t));
    if (!primes) exit(1);

    /* Allocate sieve arrays */
    spf  = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    lpf  = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    sumd = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    cntd = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!spf || !lpf || !sumd || !cntd) exit(1);

    /* Base case for 1 */
    lpf[1] = 0; sumd[1] = 0; cntd[1] = 0;

    /* Linear sieve, simultaneously filled DP arrays */
    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            primes[primes_count++] = (uint32_t)i;
        }
        uint32_t p = spf[i];
        uint32_t m = (uint32_t)(i / p);
        if (m % p == 0) {                 /* p already counted in m */
            lpf[i]  = lpf[m];
            sumd[i] = sumd[m];
            cntd[i] = cntd[m];
        } else {                          /* new prime factor */
            lpf[i]  = (p > lpf[m]) ? p : lpf[m];
            sumd[i] = sumd[m] + p;
            cntd[i] = cntd[m] + 1;
        }

        /* propagate to multiples (linear sieve) */
        for (j = 0; j < primes_count; ++j) {
            uint64_t mult = (uint64_t)i * primes[j];
            if (mult > limit) break;
            spf[(size_t)mult] = primes[j];
            if (i % primes[j] == 0) break;
        }
    }

    /* Build the sequence from the computed data */
    seq_capacity = 2048;
    seq_terms = (seq_t*)malloc(seq_capacity * sizeof(seq_t));
    if (!seq_terms) exit(1);

    for (i = 2; i <= limit; ++i) {
        if (cntd[i] >= 3 && sumd[i] % lpf[i] == 0) {
            if (seq_size == seq_capacity) {
                size_t new_cap = seq_capacity * 2;
                seq_terms = (seq_t*)realloc(seq_terms, new_cap * sizeof(seq_t));
                if (!seq_terms) exit(1);
                seq_capacity = new_cap;
            }
            seq_terms[seq_size++] = (seq_t)i;
        }
    }

    free(primes);
    initialized = 1;
}

/*  External function that returns the n-th term of A071140  */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_sequence();
    }
    /*  n is 1‑based; assume n is within precomputed range   */
    if (n == 0 || n > seq_size) {
        /*  Out of bounds – return 0; actual caller should handle   */
        return (seq_t)0;
    }
    return seq_terms[n - 1];
}
