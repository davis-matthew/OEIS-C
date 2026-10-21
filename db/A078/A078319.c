/*
 * A078319  Sum of distinct prime factors of n*rad(n)+1, where rad = A007947
 *          (squarefree kernel).
 * https://oeis.org/A078319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of distinct prime divisors of n*rad(n)+1 (rad(n) =
 *            product of distinct primes dividing n)
 * Range:     u8 n <= 25, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"

/* Cached prime table ------------------------------------------ */
static idx_t *primes = NULL;   /* array of primes up to the build limit */
static size_t primes_cnt = 0;  /* number of primes stored */

static void init_primes(void)
{
    if (primes != NULL)
        return;                /* already initialized */
    /* Build enough primes for the largest m = n*rad(n)+1 with n ≤ 10000.
       The maximum m is < (10000)*(10000)+1 = 1 000 000 001,
       so sqrt(m) < 31623.  Build a bit over that. */
    idx_t limit = 33000;       /* safe upper bound */
    build_primes(limit, &primes, &primes_cnt);
}

/* Compute the square‑free kernel rad(n). --------------------- */
static uint64_t rad_of(idx_t n)
{
    if (n <= 1)
        return 1;
    uint64_t nn = n;
    uint64_t rad = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > nn)
            break;
        if (nn % p == 0) {
            rad *= p;
            while (nn % p == 0)
                nn /= p;
        }
    }
    if (nn > 1)          /* remaining prime factor */
        rad *= nn;
    return rad;
}

/* Compute sum of distinct prime divisors of x. --------------- */
static uint64_t sum_distinct_prime_factors(uint64_t x)
{
    uint64_t sum = 0;
    uint64_t t = x;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > t)
            break;
        if (t % p == 0) {
            sum += p;
            while (t % p == 0)
                t /= p;
        }
    }
    if (t > 1)          /* remaining prime factor */
        sum += t;
    return sum;
}

/* Main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for the offset 1 */

    init_primes();                     /* ensure primes are ready */

    uint64_t rad = rad_of(n);          /* rad(n) */
    uint64_t m   = (uint64_t)n * rad + 1ULL;   /* n*rad(n)+1 */

    uint64_t sum = sum_distinct_prime_factors(m);
    return (seq_t)sum;                 /* cast to the build’s seq_t type */
}
