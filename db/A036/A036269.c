/*
 * A036269  8th differences of primes.
 * https://oeis.org/A036269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0}^8 (-1)^{8-k} * C(8,k) * prime(n+k)
 * Range:     i8 n <= 5, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where prime(m) is the m-th prime (prime(1)=2). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const seq_t COEFFS[9] = {1, -8, 28, -56, 70, -56, 28, -8, 1};

static uint64_t *PRIMES = NULL;
static size_t PRIME_COUNT = 0;
static const idx_t SIEVE_LIMIT = 500000;  /* enough for n up to ~40000 */

/* Build the list of primes up to SIEVE_LIMIT using a simple sieve. */
static void init_primes(void)
{
    if (PRIMES != NULL)
        return;  /* already built */

    bool *is_composite = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_composite)
        return;  /* failed to allocate; caller will fail gracefully */

    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_composite[i] = false;

    /* Estimate maximum number of primes to avoid repeated reallocs. */
    size_t estimated = SIEVE_LIMIT / 10;
    PRIMES = (uint64_t *)malloc(estimated * sizeof(uint64_t));
    if (!PRIMES)
    {
        free(is_composite);
        return;
    }

    PRIME_COUNT = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!is_composite[i])
        {
            /* store prime */
            if (PRIME_COUNT == estimated)
            {
                estimated = estimated * 2;
                uint64_t *tmp = (uint64_t *)realloc(PRIMES, estimated * sizeof(uint64_t));
                if (!tmp)
                    break;  /* out of memory, stop adding primes */
                PRIMES = tmp;
            }
            PRIMES[PRIME_COUNT++] = (uint64_t)i;

            /* Mark multiples */
            if (i * i <= SIEVE_LIMIT)
            {
                for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    free(is_composite);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but return 0 to avoid UB */

    init_primes();

    /* The sieve limit (500k) guarantees enough primes for n up to ≈ 41500.
       For the requested range (n up to about 10000) we are safe. */
    if (n + 8 > PRIME_COUNT)
        return 0;  /* out of bounds – could handle by extending sieve */

    seq_t result = 0;
    for (int k = 0; k < 9; ++k)
    {
        seq_t p = (seq_t)PRIMES[n + k - 1]; /* primes array is 0‑based */
        result += COEFFS[k] * p;
    }
    return result;
}
