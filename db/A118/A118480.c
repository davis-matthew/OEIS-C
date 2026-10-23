/*
 * A118480  (n-th 4k+1 prime minus n-th 4k+3 prime minus two)/4.
 * https://oeis.org/A118480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (A002144(n) - A002145(n) - 2)/4
 * Range:     i8 n <= 14, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Global state for on‑demand sieving */
static idx_t     curr_limit   = 0;   /* limit up to which primes are sieved   */
static uint32_t *primes4k1   = NULL; /* array of primes 4k+1                 */
static size_t    count4k1     = 0;   /* number of primes 4k+1 obtained       */
static uint32_t *primes4k3   = NULL; /* array of primes 4k+3                 */
static size_t    count4k3     = 0;   /* number of primes 4k+3 obtained       */

/* Simple sieve of Eratosthenes up to 'limit'.
   It collects primes == 1 (mod 4) in *out1 and primes == 3 (mod 4) in *out3. */
static void sieve_primes_upto(idx_t limit,
                              uint32_t **out1, size_t *cnt1,
                              uint32_t **out3, size_t *cnt3)
{
    /* Allocate and initialise the boolean array */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return; /* memory error – caller will not use results */

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count how many primes we need to store */
    size_t c1 = 0, c3 = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
        {
            if ((i & 3) == 1) ++c1;
            else if ((i & 3) == 3) ++c3;
        }

    /* Allocate result arrays */
    uint32_t *p1 = c1 ? (uint32_t *)malloc(c1 * sizeof(uint32_t)) : NULL;
    uint32_t *p3 = c3 ? (uint32_t *)malloc(c3 * sizeof(uint32_t)) : NULL;
    if ((c1 && !p1) || (c3 && !p3))
    {
        /* allocation failed – clean up */
        free(is_prime);
        free(p1);
        free(p3);
        return;
    }

    /* Fill the arrays */
    size_t i1 = 0, i3 = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
        {
            if ((i & 3) == 1 && i1 < c1) p1[i1++] = (uint32_t)i;
            else if ((i & 3) == 3 && i3 < c3) p3[i3++] = (uint32_t)i;
        }

    *out1 = p1;
    *cnt1 = c1;
    *out3 = p3;
    *cnt3 = c3;
    free(is_prime);
}

/* Ensure we have at least 'n' primes of each type.  If not, grow the sieve. */
static void ensure_primes(idx_t n)
{
    if (count4k1 >= n && count4k3 >= n) return; /* already enough */

    /* Release old arrays */
    free(primes4k1);
    free(primes4k3);
    primes4k1 = primes4k3 = NULL;
    count4k1 = count4k3 = 0;

    /* Choose a starting limit.  A rough bound is 20*n+100 for n up to 10 000. */
    if (curr_limit == 0)
        curr_limit = n * 20 + 100;
    else
        curr_limit = curr_limit * 2;

    while (1)
    {
        uint32_t *tmp1 = NULL;
        uint32_t *tmp3 = NULL;
        size_t c1 = 0, c3 = 0;
        sieve_primes_upto(curr_limit, &tmp1, &c1, &tmp3, &c3);

        if (!tmp1 && !tmp3) /* unlikely, but stop on memory failure */
            break;

        if (c1 >= n && c3 >= n)
        {
            primes4k1 = tmp1;
            primes4k3 = tmp3;
            count4k1 = c1;
            count4k3 = c3;
            break;
        }
        /* Not enough – free and try a larger limit */
        free(tmp1);
        free(tmp3);
        curr_limit *= 2;
    }
}

/* The OEIS sequence A118480. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* value outside offset – arbitrary */
    ensure_primes(n);
    /* indices are 0‑based internally */
    seq_t p1 = (seq_t)primes4k1[n - 1];
    seq_t p3 = (seq_t)primes4k3[n - 1];
    return ((p1 - p3 - 2) / 4);
}
