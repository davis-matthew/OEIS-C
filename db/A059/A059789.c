/*
 * A059789  Distance of 2*prime(n) from previous prime.
 * https://oeis.org/A059789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n) - previous_prime(2*prime(n)) where prime(0)=2,
 *            prime(1)=3, … The implementation generates primes with an
 *            incremental sieve and keeps them cached. For a given n it first
 *            guarantees enough primes to obtain prime(n), then extends the
 *            sieve to at least 2*prime(n), finds the first pr...
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/* Cached prime data */
static idx_t       SIEVE_LIMIT  = 0;   /* maximum value sieved so far   */
static idx_t       PRIMES_COUNT = 0;   /* number of primes recorded     */
static idx_t      *PRIMES       = NULL;/* array of primes (0‑based)     */

/* Perform a full sieve up to 'limit' (inclusive).  The sieve
   is rebuilt entirely whenever the limit increases; for the
   modest limits required by this sequence this is sufficiently
   fast. */
static void sieve_up_to(idx_t limit)
{
    idx_t i, j;
    /* allocate and initialise is_prime array */
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) abort();
    memset(is_prime, 0xFF, (limit + 1) * sizeof(bool)); /* all true */
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Eratosthenes */
    for (i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* count primes and collect them */
    idx_t cnt = 0;
    for (i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    idx_t *np = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!np) abort();
    idx_t r = 0;
    for (i = 2; i <= limit; ++i) if (is_prime[i]) np[r++] = i;

    /* replace old data */
    free(PRIMES);
    free(is_prime);
    PRIMES       = np;
    PRIMES_COUNT = cnt;
    SIEVE_LIMIT  = limit;
}

/* Ensure we have at least 'need' primes in the cache.  Uses
   Rosser–Schoenfeld upper bound for the nth prime. */
static void ensure_primes(idx_t need)
{
    if (PRIMES_COUNT >= need) return;
    idx_t bound;
    if (need < 6) {
        bound = 15;
    } else {
        double nd = (double)need;
        bound = (idx_t)(nd * (log(nd) + log(log(nd))) + 3.0);
    }
    sieve_up_to(bound);
}

/* Binary search for first prime > value */
static idx_t first_prime_gt(idx_t value)
{
    idx_t lo = 0, hi = PRIMES_COUNT;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (PRIMES[mid] <= value) lo = mid + 1;
        else                       hi = mid;
    }
    return lo; /* index of first prime > value */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* step 1: fetch prime(n) */
    ensure_primes(n + 1);             /* need primes[0..n] inclusive */
    idx_t p = PRIMES[n];              /* 0‑based index */

    /* step 2: ensure sieve covers 2*p */
    idx_t target = p << 1;
    if (target > SIEVE_LIMIT) sieve_up_to(target);

    /* step 3: locate previous prime before target */
    idx_t first_gt = first_prime_gt(target);
    idx_t prev = PRIMES[first_gt - 1]; /* there is always such a prime */

    /* step 4: return the difference as seq_t */
    return (seq_t)(target - prev);
}
