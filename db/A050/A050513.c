/*
 * A050513  a(n) = (-1)^n * Sum_{i=0..n} binomial(n+1,i+1)*prime(i+1).
 * https://oeis.org/A050513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * Σ_{i=0..n} binomial(n+1,i+1)*prime(i+1)
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 24, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----------------------------------------------------- */
/* Prime sieve – generate enough primes for n ≤ 10000   */
/* ----------------------------------------------------- */
#define PRIME_LIMIT 200000  /* >= 104729 (10000th prime) */

static int  prime_list[PRIME_LIMIT + 10];
static size_t prime_cnt = 0;
static int  primes_initialized = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    char *sieve = (char *)calloc(PRIME_LIMIT + 1, 1);
    sieve[0] = sieve[1] = 1;  /* mark 0 and 1 as non‑prime */
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 1;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) prime_list[prime_cnt++] = i;
    free(sieve);
    primes_initialized = 1;
}

/* ----------------------------------------------------- */
/* Main function                                           */
/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();

    /* Ensure we have at least (n+2) primes (0‑based array needs n+1 index) */
    if (prime_cnt <= (size_t)(n + 1))
        return (seq_t)0;  /* should not happen given PRIME_LIMIT */

    seq_t sum = 0;
    seq_t binom = (seq_t)(n + 1);  /* C(n+1,1) */

    for (idx_t j = 1; j <= n + 1; ++j) {
        sum += binom * (seq_t)prime_list[j - 1];
        if (j == n + 1) break; /* last term */
        /* Next binomial coefficient: C(n+1, j+1) = C(n+1, j) * (n+1-j)/(j+1) */
        binom = binom * (seq_t)(n + 1 - j) / (seq_t)(j + 1);
    }

    return (n & 1) ? -sum : sum;
}
