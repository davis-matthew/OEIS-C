/*
 * A226914  Third column of A226518.
 * https://oeis.org/A226914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Legendre(2,prime(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *primes = NULL;   /* dynamic array of known primes, 1‑based index */
static size_t primes_cnt = 0;     /* number of primes stored */
static size_t primes_cap = 0;     /* current capacity of 'primes' */

/* Ensure that the array contains at least 'n' primes.  */
static void _ensure_n_primes(idx_t n)
{
    if (primes_cnt >= n)
        return;

    if (primes_cnt == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();          /* allocation failure */
        primes[0] = 2;
        primes_cnt = 1;
    }

    uint64_t candidate = primes[primes_cnt - 1] + 2;
    if (candidate == 4) /* after 2 the next candidate is 3 */
        candidate = 3;
    else if ((candidate & 1) == 0)   /* make odd if necessary */
        candidate++;

    while (primes_cnt < n) {
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)          /* enough divisibility checks done */
                break;
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (primes_cnt >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_cnt++] = candidate;
        }
        candidate += 2; /* only odd candidates after 2 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1 */

    _ensure_n_primes(n);
    uint64_t p = primes[n - 1];  /* nth prime, 1‑based */

    if (p == 2)
        return (seq_t)1;          /* Legendre(2,2) is defined as 0 in OEIS context */

    uint64_t mod8 = p & 7;        /* p mod 8 */
    seq_t leg = (mod8 == 1 || mod8 == 7) ? 1 : -1;  /* Legendre symbol (2/p) */
    return (seq_t)(1 + leg);      /* a(n) = 1 + Legendre(2,prime(n)) */
}
