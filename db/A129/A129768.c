/*
 * A129768  Number of odd nonprime numbers less than the n-th prime.
 * https://oeis.org/A129768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd nonprime numbers less than the n-th prime.
 *            For n ≥ 2: a(n) = floor((p_n-1)/2) - (pi(p_n-1)-1) where p_n is
 *            the n-th prime and pi(x) is the prime counting function
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Types seq_t and idx_t are provided by the build system. */

static bool          initialized  = false;
static size_t        current_limit = 0;   /* current sieve bound          */
static idx_t *       primes        = NULL;/* array of primes              */
static size_t        primes_cnt    = 0;   /* number of primes found       */
static seq_t *       pi_arr        = NULL;/* cumulative prime counts     */

/* Estimate upper bound for the n-th prime (Rosser's theorem). */
static size_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double limit = dn * (log(dn) + log(log(dn)));
    return (size_t)limit + 3;   /* safety margin */
}

/* Perform a sieve up to the given limit and fill primes[] and pi_arr[]. */
static void make_sieve(size_t limit)
{
    /* Allocate prime flags. */
    char *comp = (char *)calloc(limit + 1, 1);
    if (!comp) return; /* allocation failure – should not happen */

    /* Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }

    /* Build pi array. */
    if (pi_arr) free(pi_arr);
    pi_arr = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!pi_arr) { free(comp); return; }
    pi_arr[0] = pi_arr[1] = 0;
    seq_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) ++cnt;
        pi_arr[i] = cnt;
    }

    /* Build list of primes. */
    if (primes) free(primes);
    size_t est = limit / (log((double)limit) + 1.0) + 10; /* rough upper bound */
    primes = (idx_t *)malloc(est * sizeof(idx_t));
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (primes_cnt >= est) {
                est <<= 1;
                primes = (idx_t *)realloc(primes, est * sizeof(idx_t));
            }
            primes[primes_cnt++] = (idx_t)i;
        }
    }

    free(comp);
    current_limit = limit;
    initialized   = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    /* Special case n=1. */
    if (n == 1) return (seq_t)1;

    /* Ensure we have enough primes for this n. */
    if (!initialized) {
        size_t limit = estimate_limit(n);
        make_sieve(limit);
    }

    while (primes_cnt < (size_t)n) {
        size_t new_limit = current_limit * 2;
        if (new_limit <= current_limit) new_limit = current_limit + 1000;
        make_sieve(new_limit);
    }

    /* p_n is the n-th prime (0‑indexed). */
    idx_t p_n = primes[n - 1];

    /* Count odd numbers < p_n. */
    seq_t odd_numbers = (seq_t)((p_n - 1) / 2);

    /* Count odd primes < p_n. */
    seq_t odd_prime_count = pi_arr[p_n - 1];
    if (p_n > 2) odd_prime_count -= 1;   /* exclude the even prime 2 */

    return odd_numbers - odd_prime_count;
}
