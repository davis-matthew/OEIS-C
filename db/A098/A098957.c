/*
 * A098957  Decimal value of the reverse binary expansion of the prime
 *          numbers.
 * https://oeis.org/A098957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 6542, u64 n <= 6542,
 *            u128 n <= 6542, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = decimal(reverse(binary(prime(n)))) */

/* The algorithm:
   1. Estimate an upper bound on the nth prime with the Rosser–Schoenfeld
      bound p_n < n (log n + log log n) for n ≥ 6.
   2. Sieve of Eratosthenes up to that bound to generate all primes.
   3. Store the primes in a static array; regenerate the array only if a
      larger n is requested.
   4. For the desired prime, reverse its binary representation bit‑by‑bit
      to produce the sequence term.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ---------- helper: estimate an upper bound for the n-th prime ------------- */
static idx_t estimate_prime_limit(idx_t n)
{
    if (n < 6)
        return 15;                         /* first 6 primes are ≤ 13 */
    double nn   = (double)n;
    double ln   = log(nn);
    double lln  = log(ln);
    double est  = nn * (ln + lln);         /* Rosser bound */
    /* Add a safety margin: 10% */
    return (idx_t)(est * 1.10 + 10.0);
}

/* ---------- static state for prime generation ----------------------------- */
static idx_t *prime_arr   = NULL;   /* array of primes */
static size_t prime_len   = 0;      /* number of primes stored */
static idx_t  current_limit = 0;    /* last sieve limit */

/* ---------- ensure we have at least n primes available ---------------------
 *     Regenerates the sieve only if the current limit is insufficient.      */
static void ensure_primes_up_to(idx_t n)
{
    if (prime_len >= n && current_limit > 0)
        return;                      /* already have enough primes */

    /* Recompute an upper bound. Keep attempting with increased bounds until
       the sieve contains at least n primes. */
    idx_t limit = estimate_prime_limit(n);

    /* Allocate temporary sieve array on the heap */
    while (true) {
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime)
            return;                /* unlikely: abort on allocation failure */

        /* initialize */
        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        /* Sieve of Eratosthenes */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }

        /* Count primes and store them if enough */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                ++cnt;

        if (cnt >= n) {
            /* Free existing array if any and allocate new one */
            if (prime_arr)
                free(prime_arr);
            prime_arr = (idx_t *)malloc(cnt * sizeof(idx_t));
            if (!prime_arr) {
                free(is_prime);
                return;            /* abort on allocation failure */
            }

            size_t pos = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (is_prime[i])
                    prime_arr[pos++] = i;

            prime_len = cnt;
            current_limit = limit;
            free(is_prime);
            break;                 /* success */
        }

        /* Not enough primes: enlarge the limit and try again */
        free(is_prime);
        limit = (idx_t)(limit * 1.10 + 100); /* increase by >10% */
    }
}

/* ---------- sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0, safe to return 0 */

    ensure_primes_up_to(n);
    seq_t p   = (seq_t)prime_arr[n - 1];    /* nth prime (1‑based) */
    seq_t rev = 0;
    /* Reverse the binary representation bit‑by‑bit */
    while (p) {
        rev = (rev << 1) | (p & 1);
        p >>= 1;
    }
    return rev;
}
