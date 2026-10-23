/*
 * A187129  Consider all pairs of primes (p,q) with p+q = 2n, p <= q; a(n) is
 *          the sum of all the q's.
 * https://oeis.org/A187129
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all q such that there exist primes p,q with p <= q
 *            and p+q = 2n
 * Range:     u8 n <= 31, u16 n <= 659, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime sieve data.  sieve[i] == 1  iff i is prime (for 0 <= i). */
static uint8_t *sieve = NULL;   /* growing array */
static idx_t sieve_max = 0;     /* largest index currently sieved */

/* Build or extend the sieve up to at least `limit`. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_max)      /* already large enough */
        return;

    /* Allocate a new array (one byte per integer). */
    uint8_t *new_sieve = (uint8_t*)malloc((limit + 1) * sizeof *new_sieve);
    if (!new_sieve)              /* allocation failed – caller will undercount */
        return;

    /* Initially mark all as prime. */
    for (idx_t i = 0; i <= limit; ++i)
        new_sieve[i] = 1;

    /* 0 and 1 are not prime. */
    if (limit >= 0) new_sieve[0] = 0;
    if (limit >= 1) new_sieve[1] = 0;

    /* Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (new_sieve[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = 0;
        }
    }

    /* Replace old sieve. */
    if (sieve)
        free(sieve);
    sieve = new_sieve;
    sieve_max = limit;
}

/* Sequence function a(n).  Defined for n >= 2, returns 0 for n < 2. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    idx_t two_n = 2 * n;
    ensure_sieve(two_n);

    seq_t sum = (seq_t)0;

    /* Enumerate primes p <= n.  If q = 2n - p is prime and q >= p, add q. */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (!sieve[p])            /* p not prime */
            continue;
        idx_t q = two_n - p;
        if (q < p)                /* maintain p <= q, avoid double counting */
            continue;
        if (sieve[q])             /* q is prime */
            sum = sum + (seq_t)q; /* add to sum; works for all seq_t types */
    }

    return sum;
}
