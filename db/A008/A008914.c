/*
 * A008914  Order of simple Chevalley group G_2 (q), q = prime power.
 * https://oeis.org/A008914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let q be the n-th prime power (prime powers listed in increasing
 *            order), a(n) = q^6 * (q^2 - 1) * (q^6 - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static data for prime powers */
static bool *is_prime = NULL;
static size_t prime_limit = 2000000;       /* upper bound for sieving primes		*/
static idx_t *prime_powers = NULL;        /* array of prime powers, sorted ascending */
static size_t pp_size = 0;                /* number of elements in prime_powers		*/
static bool initialized = false;

/* comparison function for qsort */
static int cmp_idx_t(const void *a, const void *b)
{
    const idx_t *ia = (const idx_t *)a;
    const idx_t *ib = (const idx_t *)b;
    if (*ia < *ib) return -1;
    if (*ia > *ib) return 1;
    return 0;
}

/* build the list of prime powers up to prime_limit */
static void init_prime_powers(void)
{
    /* sieve of Eratosthenes up to prime_limit */
    is_prime = (bool *)malloc((prime_limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);                       // minimal error handling

    for (size_t i = 0; i <= prime_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= prime_limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= prime_limit; m += p)
                is_prime[m] = false;
        }
    }

    /* allocate containers for primes and prime powers */
    size_t primes_capacity = 0, primes_count = 0;
    idx_t *primes = NULL;
    /* first pass: count primes to allocate exact size */
    for (size_t i = 2; i <= prime_limit; ++i)
        if (is_prime[i]) ++primes_count;
    primes_capacity = primes_count;
    primes = (idx_t *)malloc(primes_capacity * sizeof(idx_t));
    if (!primes) exit(1);

    for (size_t i = 2, j = 0; i <= prime_limit; ++i)
        if (is_prime[i]) primes[j++] = (idx_t)i;

    /* generate prime powers */
    size_t pp_cap = primes_count * 10 + 64;
    prime_powers = (idx_t *)malloc(pp_cap * sizeof(idx_t));
    if (!prime_powers) exit(1);
    pp_size = 0;

    for (size_t i = 0; i < primes_count; ++i) {
        idx_t p = primes[i];
        idx_t val = p;
        while (val <= prime_limit) {
            if (pp_size == pp_cap) {
                pp_cap <<= 1;
                prime_powers = (idx_t *)realloc(prime_powers, pp_cap * sizeof(idx_t));
                if (!prime_powers) exit(1);
            }
            prime_powers[pp_size++] = val;
            if (val > prime_limit / p) break;
            val *= p;
        }
    }

    /* sort the list */
    qsort(prime_powers, pp_size, sizeof(idx_t), cmp_idx_t);

    /* free temporary arrays */
    free(primes);
    /* is_prime can be freed if memory is critical, but keep it for quick re-use
       freeing to reduce memory footprint:
    free(is_prime);
    is_prime = NULL;
    */
    initialized = true;
}

/* the sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        init_prime_powers();

    /* safety: enlarge prime_limit if needed (rare for n <= 10000) */
    if (n == 0 || n > pp_size) {
        /* not enough prime powers precomputed; extend limit */
        /* increase limit by a factor of 2 until enough */
        while (n > pp_size) {
            size_t new_limit = prime_limit << 1;
            /* Free old data to recompute */
            free(is_prime);
            free(prime_powers);
            prime_limit = new_limit;
            init_prime_powers();
        }
    }

    idx_t q = prime_powers[n - 1];      /* nth prime power (1‑based) */

    seq_t q_sq  = (seq_t)q * (seq_t)q;
    seq_t q_cu  = q_sq * (seq_t)q;      /* q^3 */
    seq_t q6    = q_cu * q_cu;          /* q^6 */
    seq_t ans   = q6 * (q_sq - 1) * (q6 - 1);

    return ans;
}
