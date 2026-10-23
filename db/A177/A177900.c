/*
 * A177900  Smallest positive integer k such that the sum of the proper
 *          divisors of k gives the n-th prime, or 0 if prime(n) belongs to
 *          A005114.
 * https://oeis.org/A177900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k such that the sum of the
 *            proper divisors of k equals the n‑th prime, or 0 if no such k
 *            exists
 * Range:     u8 n <= 16, u16 n <= 375, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state -------------------------------------------------------- */

static bool          initialized      = false;   /* one‑time init flag */

static const idx_t   PRIME_LIMIT      = 200000;   /* primes up to about 2×10⁵  */
static const idx_t   K_LIMIT          = 4000000;  /* range for k up to ~4×10⁶ */

static unsigned char *prime_flag    = NULL;      /* 0/1 flag for primes   */
static uint64_t      *primes_arr    = NULL;      /* list of primes        */
static idx_t          primes_cnt    = 0;          /* number of primes      */

static seq_t         *first_k_of_prime = NULL;  /* prime → minimal k     */
static idx_t         *proper_sums     = NULL;  /* sum of proper divisors*/

/* helper: perform one‑time initialization ---------------------------- */
static void init_once(void)
{
    if (initialized)
        return;

    /* 1. Allocate and sieve primes up to PRIME_LIMIT */
    prime_flag = (unsigned char*)calloc(PRIME_LIMIT + 1, sizeof(unsigned char));
    if (!prime_flag)
        return;   /* out of memory – subsequent calls will return 0 */

    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) prime_flag[i] = 1;
    for (idx_t i = 2; (idx_t)i * i <= PRIME_LIMIT; ++i)
        if (prime_flag[i])
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                prime_flag[j] = 0;

    /* count primes */
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_flag[i]) ++primes_cnt;

    /* allocate and fill array of primes */
    primes_arr = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes_arr)
        return;
    idx_t idx = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_flag[i])
            primes_arr[idx++] = (uint64_t)i;

    /* 2. Allocate mapping array (prime → minimal k) */
    first_k_of_prime = (seq_t*)calloc(PRIME_LIMIT + 1, sizeof(seq_t));
    if (!first_k_of_prime)
        return;

    /* 3. Allocate array for sums of proper divisors */
    proper_sums = (idx_t*)calloc(K_LIMIT + 1, sizeof(idx_t));
    if (!proper_sums)
        return;

    /* 4. Compute sums via sieve‑like method */
    for (idx_t i = 1; i <= K_LIMIT / 2; ++i)
        for (idx_t j = i * 2; j <= K_LIMIT; j += i)
            proper_sums[j] += i;

    /* 5. Map each prime to the smallest k found */
    for (idx_t k = 1; k <= K_LIMIT; ++k) {
        idx_t s = proper_sums[k];
        if (s <= PRIME_LIMIT && prime_flag[s]) {
            if (first_k_of_prime[(idx_t)s] == 0)
                first_k_of_prime[(idx_t)s] = (seq_t)k;
        }
    }

    initialized = true;
}

/* -------------------------------------------------------------------- */
/*  The requested OEIS sequence function                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();   /* ensure the tables are ready */

    if (n == 0 || n > primes_cnt)
        return (seq_t)0;           /* out of range */

    uint64_t p = primes_arr[n - 1];     /* n‑th prime (1‑based) */

    if (p > PRIME_LIMIT)               /* prime beyond precomputation */
        return (seq_t)0;

    return first_k_of_prime[(idx_t)p];  /* 0 if no k found */
}
