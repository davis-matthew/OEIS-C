/*
 * A102552  a(n) = prime(n) - (prime(n+1) + prime(n-1))/2.
 * https://oeis.org/A102552
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n) - (prime(n+1) + prime(n-1))/2 (OEIS A102552)
 * Range:     i8 n <= 18, i16 n <= 2357, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 8510 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

/* -------------------------------------------------------------------- */
static uint64_t estimate_prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;  /* covers first 6 primes                        */
    double nn    = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (uint64_t)(bound + 3.0);  /* safety margin                         */
}

/* Static state: primes[0] = 2, primes[1] = 3, ... */
static seq_t *primes = NULL;
static size_t prime_count = 0;

/* Ensure that primes array contains at least 'required' primes (zero‑based). */
static void ensure_primes(idx_t required)
{
    if (prime_count >= (size_t)required)
        return;                     /* already enough primes */

    /* Upper bound for the 'required'-th prime (excludes the first prime). */
    idx_t bound = estimate_prime_upper_bound(required);

    /* Allocate raw table. */
    char *is_prime = (char *)calloc((size_t)bound + 1, 1);
    if (!is_prime)
        return;                    /* allocation failure – give up */

    /* Initially mark every candidate >=2 as prime */
    for (idx_t i = 2; i <= bound; ++i)
        is_prime[i] = 1;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= bound; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= bound; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to know how much space to allocate */
    size_t count = 0;
    for (idx_t i = 2; i <= bound; ++i)
        if (is_prime[i])
            ++count;

    /* Allocate storage for the primes */
    seq_t *new_primes = mem_new(seq_t, count);
    if (!new_primes) {
        free(is_prime);
        return;                    /* allocation failure */
    }

    /* Fill the prime array */
    size_t idx = 0;
    for (idx_t i = 2; i <= bound; ++i) {
        if (is_prime[i]) {
            new_primes[idx++] = (seq_t)i;
        }
    }

    /* Replace the old array if present */
    mem_free(primes);
    primes = new_primes;
    prime_count = count;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* below the OEIS offset */

    /* Need prime(n), prime(n+1) and prime(n-1). */
    ensure_primes(n + 1);                /* provides primes[0 .. n] */

    seq_t p_n   = primes[n - 1];         /* prime(n)      */
    seq_t p_np1 = primes[n];             /* prime(n+1)    */
    seq_t p_nm1 = primes[n - 2];         /* prime(n-1)    */

    seq_t sum = p_np1 + p_nm1;           /* (prime(n+1)+prime(n-1)) */
    seq_t avg = sum / 2;                 /* integer division (safe – all values positive) */

    return p_n - avg;
}
