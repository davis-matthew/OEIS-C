/*
 * A162318  A positive integer n is included if |d(n+1)-d(n)| = 2, where d(n)
 *          is the number of divisors of n.
 * https://oeis.org/A162318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer m (with offset 1) such that *
 *            |d(m+1) - d(m)| = 2, where d(k) is the number of divisors of k
 * Range:     u8 n <= 47, u16 n <= 5238, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/***************  helpers for divisor counts  ****************/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* prime list (uint32_t) */
static uint32_t *primes = NULL;
static size_t primes_len   = 0;
static size_t primes_cap   = 0;
#define PRIME_LIMIT  100000  /* sufficient for n <= 10000  */

/* initialise the prime list once */
static void init_primes(void)
{
    if (primes_len != 0) return;          /* already initialised */

    idx_t limit = PRIME_LIMIT;
    uint8_t *is_composite = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_composite) abort();

    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) {
            if (primes_cap == primes_len) {
                size_t newcap = primes_cap ? primes_cap * 2 : 8;
                uint32_t *tmp = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = newcap;
            }
            primes[primes_len++] = (uint32_t)i;

            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    free(is_composite);
}

/* fast divisor count using the pre‑computed prime list */
static uint32_t divisor_count(idx_t n)
{
    init_primes();                       /* ensure primes are ready   */
    if (n <= 1) return 1;                /* d(1) = 1, d(0)=0 (unused) */
    idx_t tmp = n;
    uint32_t res = 1;
    for (size_t i = 0; i < primes_len && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
    if (tmp > 1) res *= 2;               /* remaining prime factor   */
    return res;
}

/*********************** main sequence function ***********************/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* offset 1 – a(1) is first listed value */
    size_t   found = 0;
    idx_t    m     = 1;

    while (1) {
        uint32_t d1 = divisor_count(m);
        uint32_t d2 = divisor_count(m + 1);
        uint32_t diff = (d1 > d2) ? (d1 - d2) : (d2 - d1);
        if (diff == 2) {
            ++found;
            if (found == n) {
                return (seq_t)m;
            }
        }
        ++m;
    }
}
