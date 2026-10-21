/*
 * A238134  Number of primes p < n with q = floor((n-p)/4) and prime(q) - q +
 *          1 both prime.
 * https://oeis.org/A238134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that * q = floor((n-p)/4) is
 *            prime and prime(q) - q + 1 is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Sieve of Eratosthenes – primes up to a fixed limit                    */
/* --------------------------------------------------------------------- */

static const idx_t SIEVE_LIMIT = 200000;   /* enough for n up to ~10 000 */

static bool *prime_flag = NULL;          /* prime_flag[i] == true iff i is prime */
static idx_t *prime_list = NULL;          /* prime_list[k] == (k+1)-th prime   */
static size_t prime_cnt = 0;              /* number of primes stored          */
static bool sieve_done = false;

static void init_sieve(void)
{
    if (sieve_done)
        return;

    size_t i, j;

    /* Boolean array for primality status. */
    prime_flag = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    for (i = 0; i <= (size_t)SIEVE_LIMIT; ++i)
        prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;

    for (i = 2; i * i <= (size_t)SIEVE_LIMIT; ++i)
        if (prime_flag[i])
            for (j = i * i; j <= (size_t)SIEVE_LIMIT; j += i)
                prime_flag[j] = false;

    /* Allocate array for the primes themselves. */
    prime_list = (idx_t *)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    prime_cnt = 0;
    for (i = 2; i <= (size_t)SIEVE_LIMIT; ++i)
        if (prime_flag[i])
            prime_list[prime_cnt++] = (idx_t)i;

    sieve_done = true;
}

/* --------------------------------------------------------------------- */
/*  OEIS sequence A238134                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    init_sieve();

    seq_t count = 0;

    /* Iterate over all primes p < n. */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = prime_list[idx];
        if (p >= n)
            break;

        /* q = floor((n - p) / 4) */
        idx_t q = (n - p) / 4;
        if (q < 2)
            continue;
        if (!prime_flag[q])          /* q must be prime */
            continue;

        /* prime(q) – the q‑th prime (1‑based index) */
        if (q > (idx_t)prime_cnt)
            continue;
        idx_t prime_of_q = prime_list[q - 1];

        /* r = prime(q) - q + 1 */
        idx_t r = prime_of_q - q + 1;
        if (r < 2)
            continue;

        /* primality test for r */
        bool r_is_prime = false;
        if (r <= (idx_t)SIEVE_LIMIT) {
            r_is_prime = prime_flag[r];
        } else {
            /* trial division for values outside the pre‑computed range */
            r_is_prime = true;
            for (idx_t d = 2; d * d <= r; ++d) {
                if (r % d == 0) {
                    r_is_prime = false;
                    break;
                }
            }
        }
        if (!r_is_prime)
            continue;

        ++count;
    }

    return count;
}
