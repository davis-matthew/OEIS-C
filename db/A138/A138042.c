/*
 * A138042  Numbers k such that A096379(k)=A096379(k+1).
 * https://oeis.org/A138042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where prime(k) = 2*prime(k+2) - prime(k+3)
 * Range:     u8 n <= 8, u16 n <= 483, u32 n <= 1192, u64 n <= 1192,
 *            u128 n <= 1192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          The sequence returns the integer k itself.            */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIME_LIMIT 5000000   /* enough for n up to about 10000 */
#define PRIME_CAP   400000    /* primes up to PRIME_LIMIT count */
#define K_CAP       200000    /* storage for found k's */

static uint32_t *primes = NULL;   /* holds prime values */
static size_t primes_len = 0;     /* number of primes stored */

static uint32_t *klist   = NULL;  /* holds k indices satisfying condition */
static size_t klist_len = 0;      /* number of k's found */

static bool primes_ready = false;
static uint32_t current_k = 0;

/* Utility to generate all primes up to PRIME_LIMIT using a simple sieve. */
static void init_primes(void)
{
    if (primes_ready) return;

    /* Allocate storage */
    primes   = (uint32_t *)malloc(PRIME_CAP * sizeof(uint32_t));
    if (!primes) return;            /* allocation failed */

    klist = (uint32_t *)malloc(K_CAP * sizeof(uint32_t));
    if (!klist) { free(primes); return; }

    /* Sieve of Eratosthenes */
    unsigned char *is_composite = (unsigned char *)calloc(PRIME_LIMIT + 1, 1);
    if (!is_composite) { free(primes); free(klist); return; }

    primes_len = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_len < PRIME_CAP)
                primes[primes_len++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }

    free(is_composite);
    primes_ready = true;
    klist_len   = 0;
    current_k   = 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure primes and data structures are initialized */
    init_primes();

    /* Generate k's until we have at least n of them */
    while (klist_len < n) {
        ++current_k;
        /* Stop if we run out of precomputed primes (should not happen for n <= 10000) */
        if ((size_t)current_k + 3 >= primes_len) break;

        seq_t pk   = (seq_t)primes[current_k - 1];   /* prime(k)   */
        seq_t pk2  = (seq_t)primes[current_k + 1];   /* prime(k+2) */
        seq_t pk3  = (seq_t)primes[current_k + 2];   /* prime(k+3) */

        /* Check the defining condition */
        if (pk == 2 * pk2 - pk3) {
            if (klist_len < K_CAP)
                klist[klist_len++] = current_k;
        }
    }

    /* Return the requested term (1‑based index) */
    return (seq_t)klist[n - 1];
}
