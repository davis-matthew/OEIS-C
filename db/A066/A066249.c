/*
 * A066249  Inverse permutation to A066248.
 * https://oeis.org/A066249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n%2==0) ? A000040(n/2) : A002808((n+1)/2)) - 1
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000040 = primes, A002808 = composite numbers (>=4) */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIEVE_LIMIT 200000          /* enough for n up to ~10 000 */

static bool  *is_prime = NULL;     /* primality flag array 0..SIEVE_LIMIT */
static idx_t *prime_list   = NULL; /* primes in 1‑based order */
static idx_t *composite_list = NULL; /* composites (>=4) in 1‑based order */
static size_t prime_count = 0;
static size_t composite_count = 0;
static bool sieve_inited = false;

/* One‑time sieve construction */
static void init_sieve(void)
{
    if (sieve_inited) return;
    sieve_inited = true;

    is_prime = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    prime_list       = (idx_t *)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    composite_list   = (idx_t *)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    if (!prime_list || !composite_list) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            prime_list[prime_count++] = i;
        } else if (i >= 4) {          /* composites start at 4 */
            composite_list[composite_count++] = i;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined, return 0 safely */

    init_sieve();                         /* ensure data is ready */

    if ((n & 1ULL) == 0) {                /* even: use prime list */
        idx_t k = n / 2;                  /* 1‑based index */
        if (k == 0 || k > prime_count)   /* safety, should not happen for n>=1 */
            return (seq_t)0;
        return (seq_t)prime_list[k - 1] - 1;
    } else {                              /* odd: use composite list */
        idx_t k = (n + 1) / 2;            /* 1‑based index */
        if (k == 0 || k > composite_count)
            return (seq_t)0;
        return (seq_t)composite_list[k - 1] - 1;
    }
}
