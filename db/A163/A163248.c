/*
 * A163248  Sum of the n-th composite number plus the number of composite
 *          numbers less than the n-th noncomposite number.
 * https://oeis.org/A163248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A002808(n) + A162177(n)
 * Range:     u8 n <= 52, u16 n <= 6460, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static data structures for sieving and prime/ composite lookups */
static idx_t           sieve_limit   = 0;          /* highest number sieved */
static bool           *is_prime      = NULL;       /* prime flag array */
static idx_t           *pi_prefix    = NULL;       /* prime counting function */
static uint32_t        *primes        = NULL;       /* list of primes (uint32_t) */
static size_t          primes_count  = 0;          /* number of primes stored */

/* Forward declarations */
static void ensure_sieve(idx_t limit);
static void ensure_sieve_enough(idx_t n);
static idx_t nth_composite(idx_t n);

/* ------------------------------------------------------------------ */
/* Sieve the range [0..limit] and populate primes list and pi_prefix  */
static void ensure_sieve(idx_t limit)
{
    if (limit == 0) return;
    if (limit <= sieve_limit) return;               /* nothing to do */

    /* free old data */
    free(is_prime);
    free(pi_prefix);
    free(primes);

    /* allocate new arrays */
    is_prime   = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    pi_prefix  = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!is_prime || !pi_prefix) {                 /* allocation failed */
        exit(1);
    }

    /* initialise all as potentially prime */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t multiple = p * p; multiple <= limit; multiple += p) {
                is_prime[multiple] = false;
            }
        }
    }

    /* build prefix count of primes */
    pi_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        pi_prefix[i] = pi_prefix[i - 1] + (is_prime[i] ? 1 : 0);
    }

    /* extract prime list */
    primes_count = (size_t)pi_prefix[limit];
    primes = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            primes[idx++] = (uint32_t)i;
        }
    }

    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that the sieve covers enough numbers to support
   computing the n-th composite and the (n-1)-th prime.          */
static void ensure_sieve_enough(idx_t n)
{
    if (n == 0) return;
    if (n == 1) {                     /* very small case - just need a few */
        if (sieve_limit < 10) ensure_sieve(20);
        return;
    }

    idx_t limit = (sieve_limit > 0) ? sieve_limit : 20;

    /* Expand until we have at least n composites and n-1 primes */
    while (1) {
        ensure_sieve(limit);

        /* number of composites up to limit = limit - pi(limit) - 1  (for n>1)^ */
        idx_t composites = 0;
        if (limit >= 2) composites = limit - pi_prefix[limit] - 1;

        if (composites >= n && pi_prefix[limit] >= n - 1) break;
        limit = limit * 2 + 100;
    }
}

/* ------------------------------------------------------------------ */
/* Return the n-th composite number (n>=1).                          */
static idx_t nth_composite(idx_t n)
{
    idx_t count = 0;
    for (idx_t i = 4; i <= sieve_limit; ++i) {
        if (!is_prime[i]) {
            ++count;
            if (count == n) return i;
        }
    }

    /* Should never reach here if sieve limit is sufficient. */
    return 0;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 0 */
    if (n == 1) return (seq_t)4;         /* first term known */

    /* Ensure sufficient data */
    ensure_sieve_enough(n);

    /* n-th composite */
    idx_t comp_n = nth_composite(n);

    /* (n)-th noncomposite number (i.e., 1 for n=1,
       otherwise the (n-1)th prime) */
    idx_t p_n = (n == 1) ? 1 : (idx_t)primes[n - 2];

    /* Number of primes less than p_n */
    idx_t pi_val = 0;
    if (p_n > 1) pi_val = pi_prefix[p_n - 1];

    /* Composite numbers less than p_n */
    idx_t comp_less = 0;
    if (p_n >= 2) comp_less = p_n - 2 - pi_val;   /* p_n - 2 - pi(p_n-1) */

    /* Result */
    idx_t result = comp_n + comp_less;

    return (seq_t)result;
}
