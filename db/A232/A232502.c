/*
 * A232502  Number of ways to write n = k + m (0 < k < m) with 2*prime(m) -
 *          prime(k) prime.
 * https://oeis.org/A232502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = k + m with 0 < k < m such
 *            that 2*prime(m) - prime(k) is prime. Offset: 1 (a(1) is the
 *            first term)
 * Range:     u8 n <= 15, u16 n <= 1165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static data used by the sieve */
static const unsigned int LIMIT = 210000;      /* sufficient for n <= 10000 */
static unsigned char *IS_PRIME = NULL;         /* prime table */
static uint32_t *PRIMES   = NULL;              /* 1‑based list of primes */
static size_t PRIME_CNT  = 0;                  /* number of primes stored */

/* Build a simple sieve of Eratosthenes up to LIMIT and fill PRIMES. */
static void build_primes(void)
{
    if (PRIMES != NULL)            /* already built */
        return;

    IS_PRIME = (unsigned char*)malloc(LIMIT + 1);
    if (IS_PRIME == NULL) abort();

    for (unsigned int i = 0; i <= LIMIT; ++i)
        IS_PRIME[i] = 1;
    IS_PRIME[0] = 0;
    IS_PRIME[1] = 0;

    for (unsigned int i = 2; (unsigned long long)i * i <= LIMIT; ++i)
    {
        if (IS_PRIME[i])
        {
            for (unsigned int j = i * i; j <= LIMIT; j += i)
                IS_PRIME[j] = 0;
        }
    }

    /* Count primes to determine the size of PRIMES. */
    for (unsigned int i = 2; i <= LIMIT; ++i)
        if (IS_PRIME[i])
            ++PRIME_CNT;

    PRIMES = (uint32_t*)malloc((PRIME_CNT + 1) * sizeof(uint32_t)); /* 1‑based */
    if (PRIMES == NULL) abort();

    PRIMES[0] = 0;                /* unused */
    size_t idx = 1;
    for (unsigned int i = 2; i <= LIMIT; ++i)
        if (IS_PRIME[i])
            PRIMES[idx++] = i;
}

/*---------------------------------------------------------------*/
/*  OEIS A232502 computation                                        */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    build_primes();

    /* In all valid decompositions we have k < m, so k < n/2. */
    idx_t max_k = (n - 1) / 2;
    seq_t count = 0;

    for (idx_t k = 1; k <= max_k; ++k)
    {
        idx_t m = n - k;
        if ((size_t)m > PRIME_CNT)        /* m beyond stored primes */
            continue;

        seq_t pk = PRIMES[(size_t)k];     /* prime(k) */
        seq_t pm = PRIMES[(size_t)m];     /* prime(m) */

        seq_t val = (seq_t)2 * pm - pk;    /* 2*prime(m) - prime(k) */

        if (val <= (seq_t)LIMIT && IS_PRIME[(unsigned int)val])
            ++count;
    }

    return count;
}
