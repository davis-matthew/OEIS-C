/*
 * A055460  Number of primes with odd exponents in the prime power
 *          factorization of n!.
 * https://oeis.org/A055460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ n whose exponent in the prime
 *            factorization of n! is odd
 * Range:     u8 n <= 2612, u16 n <= 9764, u32 n <= 9764, u64 n <= 9764,
 *            u128 n <= 9764, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Global state for a lazy sieve of primes up to the maximum n
   requested.  The sieve is rebuilt only when a larger n is needed.
   ------------------------------------------------------------------ */
static uint32_t *primes   = NULL;   /* array of primes ≤ sieve_lim      */
static size_t   primes_cnt = 0;     /* current number of primes         */
static size_t   primes_alloc = 0;   /* allocated size of primes array   */
static size_t   sieve_lim   = 0;    /* current upper bound of the sieve */

/* ------------------------------------------------------------------
   Ensure that all primes ≤ limit are available in the global array.
   ------------------------------------------------------------------ */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= (idx_t)sieve_lim)
        return;            /* nothing to do */

    /* Allocate a boolean array for sieve */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)               /* allocation failed: abort */
        return;

    /* Initialise */
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    if (sz > 0) is_prime[0] = false;
    if (sz > 1) is_prime[1] = false;

    /* Perform a simple Eratosthenes sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes and rebuild the primes array if necessary */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    if (primes_alloc < cnt) {
        uint32_t *tmp = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
        if (!tmp) {   /* realloc failed: abort */
            free(is_prime);
            return;
        }
        primes = tmp;
        primes_alloc = cnt;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[idx++] = (uint32_t)i;

    primes_cnt = idx;
    sieve_lim  = limit;

    free(is_prime);
}

/* ------------------------------------------------------------------
   Compute the required value for a given n.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)                         /* n = 1 → a(1) = 0 */
        return (seq_t)0;

    _ensure_primes_upto(n);

    seq_t count = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if (p > n) break;              /* safety check */

        idx_t t = n / p;
        seq_t parity = 0;
        while (t) {
            parity ^= (t & 1);          /* toggle parity if t is odd */
            t /= p;
        }
        if (parity)
            ++count;
    }

    return count;
}
