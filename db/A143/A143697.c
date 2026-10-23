/*
 * A143697  Least square k^2 such that n^2-k^2 = p*q with p and q odd primes
 *          and p<q for n>= 4.
 * https://oeis.org/A143697
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k² < n² such that n² – k² = p·q * where p
 *            and q are distinct odd primes (p < q). If no such k exists *
 *            a(n) is returned as 0 (this never happens for n ≥ 4)
 * Range:     u8 n <= 8, u16 n <= 102, u32 n <= 9491, u64 n <= 9491,
 *            u128 n <= 9491, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
/* Use the utility that provides an integer square‑root function */
#include "../utils/floor_sqrt.h"

/* Globals -------------------------------------------------------------- */
static uint32_t primes[20001];    /* primes up to 20000 (sufficient for n ≤ 10⁴) */
static size_t   primes_cnt = 0;   /* number of primes stored */
static idx_t    sieve_limit = 0;  /* highest value for which the sieve is built */

/* -------------------------------------------------------------------- */
/* Build a simple sieve of Eratosthenes up to at least 'limit'.        */
/* -------------------------------------------------------------------- */
static void build_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                       /* nothing to do */

    if (limit > 20000)
        limit = 20000;                /* we only support primes up to 20000 */

    bool *comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!comp)
        return;                       /* allocation failed – leave empty */

    primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            primes[primes_cnt++] = (uint32_t)i;

            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    comp[j] = true;
            }
        }
    }

    sieve_limit = limit;
    free(comp);
}

/* -------------------------------------------------------------------- */
/* Test whether x is a prime.  Uses the primes[] array (sufficient for */
/* x ≤ 1e8 because sqrt(x) ≤ 10⁴).                                       */
/* -------------------------------------------------------------------- */
static bool prime_test(seq_t x)
{
    if (x < 2)
        return false;

    seq_t sq = floor_sqrt(x);
    for (size_t i = 0; i < primes_cnt && primes[i] <= sq; ++i) {
        if (x % primes[i] == 0)
            return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/* Main sequence function.                                                */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                       /* undefined below the offset */

    build_primes_upto(n);                /* primes up to n are needed */

    seq_t n_sq = (seq_t)n * (seq_t)n;

    for (idx_t k = 1; k < n; ++k) {
        if ((n & 1) == (k & 1))
            continue;                   /* parity must differ → diff odd */

        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t diff = n_sq - k_sq;        /* n² – k², guaranteed odd */

        /* Factorise diff as a product of two distinct odd primes.     */
        seq_t sq_diff = floor_sqrt(diff);
        seq_t p = 0;
        for (size_t i = 0; i < primes_cnt && primes[i] <= sq_diff; ++i) {
            seq_t prime = (seq_t)primes[i];
            if (diff % prime == 0) {
                p = prime;
                break;
            }
        }

        if (p == 0)
            continue;                   /* diff itself is prime → reject */

        seq_t q = diff / p;
        if (q == p)
            continue;                   /* not distinct */

        if (!prime_test(q))
            continue;                   /* q not prime */

        /* All conditions satisfied; return k². */
        return k_sq;
    }

    return 0;   /* Theoretically unreachable for n ≥ 4. */
}
