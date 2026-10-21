/*
 * A071592  Numbers m such that Reversal(m) is not squarefree.
 * https://oeis.org/A071592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th integer m > 0 such that reverse(m) is NOT
 *            squarefree
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: scan positive integers in increasing order, test whether the
   decimal reversal of each number contains a repeated prime factor.  A simple
   sieve of Eratosthenes precomputes all primes up to 1 000 000; this is
   sufficient for checking squarefree status up to the values needed for
   n ≤ 10 000.  Each call to a(idx_t n) computes the term independently;
   therefore repeated calls with the same n produce the same value. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------ */
/* Reverse a non‑negative integer.                                           */
/* ------------------------------------------------------------------------ */
static uint64_t reverse_uint64(uint64_t x)
{
    uint64_t rev = 0;
    while (x) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* ------------------------------------------------------------------------ */
/* Prime sieve: precompute all primes ≤ 1,000,000                           */
/* ------------------------------------------------------------------------ */
#define SIEVE_LIMIT 1000000U

static uint64_t *prime_list = NULL;
static size_t     prime_cnt  = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;

    bool *sieve = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!sieve)
        return; /* very unlikely in the test harness */

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        sieve[i] = false;

    /* Over‑allocate the array to avoid realloc during the sieve. */
    prime_list = (uint64_t *)malloc((SIEVE_LIMIT / 10) * sizeof(uint64_t));
    prime_cnt  = 0;

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            /* add prime */
            prime_list[prime_cnt++] = i;
            if (i * i <= SIEVE_LIMIT) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
}

/* ------------------------------------------------------------------------ */
/* Check whether a number is squarefree (no square divisor > 1).           */
/* ------------------------------------------------------------------------ */
static bool is_squarefree_uint64(uint64_t x)
{
    if (x <= 1)
        return true;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p  = prime_list[i];
        uint64_t p2 = p * p;
        if (p2 > x)
            break;

        if (x % p == 0) {
            uint64_t div = x / p;
            if (div % p == 0)
                return false; /* p^2 divides x */
        }
    }
    return true;
}

/* ------------------------------------------------------------------------ */
/* Public interface: compute the n‑th term of the sequence.                 */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure primes are available. */
    init_primes();

    uint64_t candidate = 1;
    idx_t    count     = 0;

    while (count < n) {
        uint64_t rev = reverse_uint64(candidate);
        if (!is_squarefree_uint64(rev))
            ++count;
        ++candidate;
    }

    /* candidate was incremented once after reaching the n‑th term */
    return (seq_t)(candidate - 1);
}
