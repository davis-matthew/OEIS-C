/*
 * A056929  Difference between n^2 and average of smallest prime greater than
 *          n^2 and largest prime less than n^2.
 * https://oeis.org/A056929
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^2 - (prev_prime(n^2) + next_prime(n^2))/2 where
 *            prev_prime(x) is the largest prime < x and next_prime(x) is the
 *            smallest prime > x
 * Range:     i8 n <= 11, i16 n <= 603, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* minimal includes that are available in both C11 and C++17 */
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* ------------------------------------------------------------------
 * Small prime sieve – used for trial division in the primality test
 * ------------------------------------------------------------------ */

static size_t prime_limit = 0;               /* largest number that has been sieved   */
static unsigned char *prime_sieve = NULL;    /* 0 = composite, 1 = prime              */
static uint32_t *primes = NULL;              /* list of primes                         */
static size_t primes_cnt = 0;                /* number of stored primes                */

static void init_primes(size_t limit)
{
    if (limit <= prime_limit) return;       /* already sufficient                           */

    /* free previous allocation if any */
    if (prime_sieve) free(prime_sieve);
    if (primes) free(primes);

    /* allocate sieve bitmap */
    prime_sieve = (unsigned char*)calloc(limit + 1, 1);
    if (!prime_sieve) return;                 /* allocation failed – unlikely for our limits */

    /* initially mark all >1 as potential primes */
    prime_sieve[0] = prime_sieve[1] = 0;
    for (size_t i = 2; i <= limit; ++i) prime_sieve[i] = 1;

    /* sieve of Eratosthenes */
    size_t sq = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sq; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_sieve[j] = 0;
        }
    }

    /* collect primes into an array for fast trial division */
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_sieve[i]) ++primes_cnt;

    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) return;                 /* allocation failed */

    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_sieve[i]) primes[k++] = (uint32_t)i;

    prime_limit = limit;
}

/* ------------------------------------------------------------------
 * Deterministic primality test for 64‑bit numbers
 * Uses trial division by the pre‑computed list of primes.
 * For n up to about 10^12 this is plenty fast.
 * ------------------------------------------------------------------ */

static int is_prime_uint64(uint64_t n)
{
    if (n < 2)          return 0;
    if (n == 2 || n == 3) return 1;
    if (n % 2 == 0)     return 0;

    /* Ensure the primes list is at least a few hundred primes long.  */
    if (prime_limit < 100000) init_primes(100000);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) return n == (uint64_t)p;
    }
    return 1;
}

/* ------------------------------------------------------------------
 * Find the largest prime strictly less than x (x > 2)
 * ------------------------------------------------------------------ */
static uint64_t prev_prime_uint64(uint64_t x)
{
    if (x <= 2) return 0;                 /* no prime below 2 */
    uint64_t y = x - 1;
    while (y >= 2) {
        if (is_prime_uint64(y)) return y;
        --y;
    }
    return 2;                              /* should never reach here */
}

/* ------------------------------------------------------------------
 * Find the smallest prime strictly greater than x
 * ------------------------------------------------------------------ */
static uint64_t next_prime_uint64(uint64_t x)
{
    uint64_t y = x + 1;
    while (1) {
        if (is_prime_uint64(y)) return y;
        ++y;
    }
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* the OEIS offset is 2; values below 2 are irrelevant – return 0 */
    if (n < 2) return 0;

    /* compute n^2 as seq_t to avoid overflow in 64‑bit types when seq_t
       is larger (e.g., 128‑bit).  For the intended test range n≤10^4,
       32‑bit is sufficient, but seq_t guarantees correctness.          */
    const seq_t sq = (seq_t)n * (seq_t)n;

    /* ensure the prime list is ready (at least enough for sqrt of n²) */
    if (prime_limit < 100000) init_primes(100000);

    /* find surrounding primes */
    uint64_t lower = prev_prime_uint64((uint64_t)sq);
    uint64_t upper = next_prime_uint64((uint64_t)sq);

    /* compute integer average of the two primes */
    seq_t avg = ((seq_t)lower + (seq_t)upper) / 2;

    /* result may be negative – seq_t is signed in the build                 */
    return sq - avg;
}
