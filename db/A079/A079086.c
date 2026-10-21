/*
 * A079086  Total number of prime factors of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigOmega( (prime(n)+1)*(prime(n+1)+1)/4 ) (the total
 *            number of prime factors, counted with multiplicity, of the
 *            product of successive primes incremented by one and then divided
 *            by four)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers – sufficient for both C11 and C++17 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Prime sieve – generate a static table of primes up to a fixed
    limit.  The prime table is large enough to satisfy typical
    queries (n up to roughly 100 000).  It is built only once.   */
/* ------------------------------------------------------------ */
#define PRIME_TABLE_CAP 2000000   /* upper bound for generated primes */
static uint32_t prime_table[PRIME_TABLE_CAP];
static uint32_t prime_count = 0;

static void init_prime_table(void)
{
    static bool done = false;
    if (done)
        return;
    done = true;

    const uint32_t LIMIT = PRIME_TABLE_CAP;
    /* Allocate the sieve – cast to uint8_t* for C++ compliance */
    uint8_t *sieve = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!sieve) {
        /* Allocation failed – program cannot continue in a meaningful way */
        abort();
    }

    /* Mark all entries as potential primes */
    for (uint32_t i = 2; i <= LIMIT; i++)
        sieve[i] = 1;

    /* Eratosthenes – eliminate composites */
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; i++)
        if (sieve[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;

    /* Store the primes in the static table */
    for (uint32_t i = 2; i <= LIMIT; i++)
        if (sieve[i])
            if (prime_count < PRIME_TABLE_CAP)
                prime_table[prime_count++] = i;
            else
                break;   /* safety – should never trigger for the chosen limit */

    free(sieve);
}

/* ------------------------------------------------------------ */
/*  Count the total (with multiplicity) number of prime factors
    of a 64‑bit unsigned integer.                                   */
/* ------------------------------------------------------------ */
static uint32_t count_prime_factors_uint64(uint64_t v)
{
    uint32_t count = 0;
    for (uint32_t i = 0; i < prime_count &&
                       (uint64_t)prime_table[i] * (uint64_t)prime_table[i] <= v; i++) {
        uint32_t p = prime_table[i];
        while (v % p == 0) {
            v /= p;
            count++;
        }
    }
    if (v > 1)          /* remaining factor is a prime > sqrt(original v) */
        count++;
    return count;
}

/* ------------------------------------------------------------ */
/*  Main sequence function – compatible with the build environment
    that supplies `seq_t` and `idx_t`.                                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined for n < 1 */

    init_prime_table();   /* ensure primes are ready */

    /* Guard against overruns – for typical problem limits this never
       triggers, but it makes the function safe for exceptional
       (large) n. */
    if ((uint64_t)n >= (uint64_t)prime_count - 1)
        return 0;    /* insufficient primes precomputed */

    /* Retrieve consecutive primes – primes are stored 0‑based */
    uint32_t p_n   = prime_table[n - 1];   /* prime(n)  */
    uint32_t p_n1  = prime_table[n];      /* prime(n+1) */

    /* Compute (p_n+1)*(p_{n+1}+1)/4.  All operations fit in 64 bits
       for the values actually needed. */
    uint64_t prod = (uint64_t)(p_n + 1) * (uint64_t)(p_n1 + 1);
    uint64_t val  = prod / 4;              /* guaranteed to be integer */

    /* Count prime factors (with multiplicity) */
    uint32_t result = count_prime_factors_uint64(val);

    return (seq_t)result;
}
