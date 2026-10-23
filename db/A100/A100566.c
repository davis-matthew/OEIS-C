/*
 * A100566  2*a(n) = prime(prime(n)) + prime(prime(2n)).
 * https://oeis.org/A100566
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   2*a(n) = prime(prime(n)) + prime(prime(2n))
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by generating all primes up to a fixed limit
   once, and then using table lookup for nth primes.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state shared by the function */
static bool *is_composite = NULL;      /* sieve of Eratosthenes */
static uint32_t *prime_arr = NULL;      /* array of primes (1‑based) */
static size_t prime_cnt = 0;            /* number of primes found */
static size_t sieve_limit = 0;          /* current sieve limit */

/* forward declarations */
static void init_sieve(void);
static uint32_t nth_prime(idx_t n);

static void init_sieve(void)
{
    /* 20 million is enough for n up to at least 100,000
       and well below the time & memory constraints.          */
    const size_t limit = 20000000;
    size_t i, j;
    sieve_limit = limit;

    is_composite = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_composite) return;       /* allocation failed */

    /* simple Eratosthenes sieve */
    for (i = 0; i <= limit; ++i) is_composite[i] = false;
    for (i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (j = i * i; j <= limit; j += i)
                is_composite[j] = true;

    /* count primes */
    prime_cnt = 0;
    for (i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++prime_cnt;

    /* allocate prime array */
    prime_arr = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_arr) return;          /* allocation failed */

    /* store primes */
    size_t idx = 0;
    for (i = 2; i <= limit; ++i)
        if (!is_composite[i])
            prime_arr[idx++] = (uint32_t)i;
}

/* 1‑based nth prime.  Assumes the sieve has been initialized. */
static uint32_t nth_prime(idx_t n)
{
    if (n < 1 || (size_t)n > prime_cnt) return 0;   /* out of bounds */
    return prime_arr[(size_t)(n - 1)];
}

seq_t a(idx_t n)
{
    /* initialise sieve on first call */
    if (!sieve_limit) init_sieve();

    if (n < 2) return 0;   /* sequence defined from n=2 */

    /* compute prime(n) and prime(2n) */
    uint32_t p_n  = nth_prime(n);
    uint32_t p_2n = nth_prime((idx_t)(2 * n));

    /* compute prime(prime(n)) and prime(prime(2n)) */
    seq_t pp_n  = (seq_t)nth_prime((idx_t)p_n);
    seq_t pp_2n = (seq_t)nth_prime((idx_t)p_2n);

    /* return a(n) = (pp_n + pp_2n) / 2 */
    return (pp_n + pp_2n) / 2;
}
