/*
 * A088739  Smallest semiprime divisor of n-th composite number.
 * https://oeis.org/A088739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Pre‑compute a sieve of smallest prime factors up to a fixed
   limit, then build the list of composite numbers.  For each query n
   (where n ≥ 1) we fetch the nth composite and return its smallest
   semiprime divisor. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define SIEVE_LIMIT 2000000   /* enough for n up to ~10⁶ composite index */

static uint32_t *spf_ptr = NULL;            /* smallest prime factor array   */
static uint32_t *composites = NULL;         /* list of composite numbers     */
static size_t composites_len = 0;           /* number of composites stored   */

/* Initialize the sieve and the composites list once. */
static void init_sieve(void)
{
    uint32_t i, p;
    size_t j, prime_cnt;

    /* Allocate and zero the smallest‑prime‑factor array. */
    spf_ptr = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));

    /* Allocate array to hold all composites up to the limit. */
    composites = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));

    /* Linear sieve to compute spf for every number ≤ SIEVE_LIMIT. */
    uint32_t *primes = (uint32_t*)malloc(SIEVE_LIMIT * sizeof(uint32_t));
    prime_cnt = 0;

    for (i = 2; i <= (uint32_t)SIEVE_LIMIT; ++i) {
        if (spf_ptr[i] == 0) {
            spf_ptr[i] = i;
            primes[prime_cnt++] = i;
        }
        for (j = 0; j < prime_cnt; ++j) {
            p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > (uint64_t)SIEVE_LIMIT) break;
            spf_ptr[(uint32_t)prod] = p;
            if (p == spf_ptr[i]) break;
        }
    }
    free(primes);

    /* Build vector of composites (>1 and not prime). */
    size_t idx = 0;
    for (i = 4; i <= (uint32_t)SIEVE_LIMIT; ++i) {
        if (spf_ptr[i] != i) {
            composites[idx++] = i;
        }
    }
    composites_len = idx;
}

/* Return the smallest semiprime divisor of a composite c. */
static inline seq_t smallest_semiprime(uint32_t c)
{
    uint32_t p = spf_ptr[c];
    uint32_t rest = c / p;
    if (spf_ptr[rest] == p) {           /* c divisible by p^2 */
        return (seq_t)((uint64_t)p * p);
    } else {                             /* smallest divisor = p * next prime factor */
        return (seq_t)((uint64_t)p * spf_ptr[rest]);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_sieve();
        initialized = 1;
    }

    if (n == 0) return (seq_t)0;                 /* safety for n=0 */
    if ((size_t)n > composites_len) return (seq_t)0;  /* out of precomputed range */

    uint32_t composite = composites[(size_t)n - 1];
    return smallest_semiprime(composite);
}
