/*
 * A178216  a(n) = prime(A178215(n)) mod prime(n).
 * https://oeis.org/A178216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A178216: for every n >= 1 let p = prime(n).  
   Find the minimal k such that the set {prime(1) mod p, …,
   prime(k) mod p} contains all residues 0 … p-1.  
   Then a(n) = prime(k) mod p.  This implementation
   precomputes all primes up to 3 000 000 and for each query
   scans the primes in order while maintaining a timestamped
   visited array to detect when all residues have appeared. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint32_t MAX_PRIME_LIMIT = 3000000;   /* > prime(10000) * log … */

/* Dynamic prime storage */
static uint32_t *primes = NULL;  /* array of primes in ascending order */
static size_t primes_count = 0;

/* Visitation timestamps for residues modulo any prime <= MAX_PRIME_LIMIT */
static int *visit_time = NULL;   /* visit_time[residue] == stamp if seen in current query */
static int visit_stamp = 0;      /* global stamp counter */

/* One‑time initialization of primes and visitation table */
static void init_primes(void)
{
    static bool initialized = false;
    if (initialized)
        return;

    /* Allocate composite flag array (bitset would be smaller but byte array is simpler) */
    unsigned char *is_comp = (unsigned char *)malloc(MAX_PRIME_LIMIT + 1);
    for (uint32_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        is_comp[i] = 0;

    /* Allocate primes array with generous capacity */
    size_t estimated = MAX_PRIME_LIMIT / 10 + 100;  /* upper bound on number of primes */
    primes = (uint32_t *)malloc(estimated * sizeof(uint32_t));
    primes_count = 0;

    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_count == estimated) { /* reallocate if capacity exceeded */
                estimated *= 2;
                primes = (uint32_t *)realloc(primes, estimated * sizeof(uint32_t));
            }
            primes[primes_count++] = i;
            if ((uint64_t)i * i <= MAX_PRIME_LIMIT)
                for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
        }
    }
    free(is_comp);

    visit_time = (int *)calloc(MAX_PRIME_LIMIT + 1, sizeof(int));
    initialized = true;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1, return 0 */

    init_primes();

    /* Ensure we have prime(n) */
    if ((size_t)(n - 1) >= primes_count)
        return (seq_t)0;            /* out of bounds, shouldn't happen with chosen limit */

    uint32_t mod_prime = primes[n - 1];
    uint32_t residues_needed = mod_prime;   /* count of distinct residues to encounter */

    /* Prepare for this query */
    ++visit_stamp;                    /* new stamp */
    int cur_stamp = visit_stamp;
    size_t unique = 0;
    size_t idx = 0;                    /* index into primes array */

    while (unique < residues_needed) {
        if (idx >= primes_count) {
            /* Should not happen with generous MAX_PRIME_LIMIT */
            return (seq_t)0;
        }
        uint32_t r = primes[idx] % mod_prime;
        if (visit_time[r] != cur_stamp) {
            visit_time[r] = cur_stamp;
            ++unique;
        }
        ++idx;
    }
    /* idx has been incremented one step too far in loop */
    --idx;                              /* last prime contributing to full coverage */
    uint32_t answer = primes[idx] % mod_prime;
    return (seq_t)answer;
}
