/*
 * A073433  Value of [p(n)+c(n)]/2 when it is an integer; p(n) is n-th prime,
 *          c(n) is n-th composite.
 * https://oeis.org/A073433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n)+c(n))/2 when the sum is even where p(n) is the n‑th
 *            prime and c(n) is the n‑th composite (composites start with 4)
 * Range:     u8 n <= 27, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------------
   Configuration limits
--------------------------------------------------------------------*/
#define SIEVE_LIMIT 200000          /* enough for n <= 10000               */
#define ARRAY_CAP   200000          /* space for primes, composites, seq   */

/*--------------------------------------------------------------------
   Storage for primes, composites and computed sequence values
--------------------------------------------------------------------*/
static uint64_t primes[ARRAY_CAP];
static size_t primes_len = 0;

static uint64_t composites[ARRAY_CAP];
static size_t composites_len = 0;

static seq_t seq[ARRAY_CAP];
static size_t seq_len = 0;               /* number of terms already computed */
static size_t pair_idx = 0;              /* index of the next (prime,composite) pair */

/*--------------------------------------------------------------------
   One‑time sieve of Eratosthenes
--------------------------------------------------------------------*/
static int initialized = 0;

static void init_sieve(void)
{
    char *is_prime = (char *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_len < ARRAY_CAP)
                primes[primes_len++] = i;
        } else if (i >= 4) {                /* composites start at 4 */
            if (composites_len < ARRAY_CAP)
                composites[composites_len++] = i;
        }
    }
    free(is_prime);
}

/*--------------------------------------------------------------------
   Main function requested
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve();
        initialized = 1;
    }

    /* If already computed enough terms, return directly */
    if (n <= seq_len)
        return seq[n - 1];

    /* Generate additional terms until we have at least n of them */
    while (seq_len < n) {
        /* Ensure we have enough pairs */
        if (pair_idx >= primes_len || pair_idx >= composites_len)
            return 0;                  /* beyond precomputed range */

        uint64_t p = primes[pair_idx];
        uint64_t c = composites[pair_idx];
        uint64_t sum = p + c;

        if ((sum & 1) == 0) {          /* integer average exists */
            seq[seq_len++] = (seq_t)(sum / 2);
        }
        pair_idx++;                    /* advance to next pair */
    }

    return seq[n - 1];
}
