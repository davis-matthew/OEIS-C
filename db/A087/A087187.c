/*
 * A087187  Number of ways to write n = i*p + j*q, where (p,q) is a twin prime
 *          pair and i,j>0.
 * https://oeis.org/A087187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to represent n = i*p + j*q with (p,q) a
 *            twin prime pair (q = p+2) and i,j > 0
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Types defined by the build */
extern idx_t _SieveMaxLimit;   /* unused placeholder to avoid warnings */

/* static sieve cache */
static idx_t    _sieve_limit = 0;
static uint8_t * _sieve      = NULL;

/* Build a simple sieve of Eratosthenes up to 'limit' (inclusive).
   The sieve array stores 0 for primes and 1 for composites. */
static void _sieve_primes_up_to(idx_t limit)
{
    if (limit <= _sieve_limit) return;
    size_t sz = (size_t)(limit + 1);
    uint8_t *new_sieve = (uint8_t *)calloc(sz, 1);
    if (!new_sieve) return;           /* allocation failure: caller will see no primes */
    new_sieve[0] = new_sieve[1] = 1;   /* 0 and 1 are not primes */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (!new_sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = 1;
    free(_sieve);
    _sieve = new_sieve;
    _sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    _sieve_primes_up_to(n);       /* ensure sieve up to n is ready */

    seq_t ways = 0;

    for (idx_t p = 2; p <= n - 2; ++p) {      /* p is the lower twin */
        if (_sieve[p]) continue;              /* skip composites */
        idx_t q = p + 2;
        if (q > n) break;                    /* twin cannot exceed n */
        if (_sieve[q]) continue;              /* skip if q is not prime */

        /* Count positive integer solutions to i*p + j*q = n */
        /* i >= 1, j >= 1 -> i <= (n - q)/p   and remaining must be multiple of q */
        idx_t max_i = (n - q) / p;
        for (idx_t i = 1; i <= max_i; ++i) {
            idx_t remaining = n - i * p;
            if (remaining < q) continue;       /* ensures j >= 1 */
            if ((remaining % q) == 0) {
                /* j = remaining / q is automatically >= 1 */
                ways++;
            }
        }
    }
    return ways;
}
