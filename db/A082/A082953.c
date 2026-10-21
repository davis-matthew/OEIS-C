/*
 * A082953  a(n) = A000252(n) / A070732(n).
 * https://oeis.org/A082953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = phi(n)*phi(2*n) = phi(n)^2 if n is odd, 2*phi(n)^2 if n
 *            is even
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes.h"

/* Static storage for primes up to the maximum sqrt(n) seen so far. */
static idx_t     *primes = NULL;   /* array of primes */
static size_t     primes_cnt = 0;   /* number of primes stored */
static idx_t     primes_limit = 0;  /* largest prime that has been generated */

/* Ensure that primes contains all primes <= limit. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;
    idx_t  *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes(limit, &new_primes, &new_cnt);
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

/* Euler's totient function for a 64‑bit integer, returning a seq_t. */
static seq_t
phi_for_n(idx_t n)
{
    seq_t phi = (seq_t)n;               /* start with n */
    idx_t n_tmp = n;
    /* Factor n using the stored primes. */
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n_tmp; ++i) {
        idx_t p = primes[i];
        if (n_tmp % p == 0) {
            while (n_tmp % p == 0)
                n_tmp /= p;
            phi -= phi / (seq_t)p;
        }
    }
    if (n_tmp > 1)          /* remaining prime factor */
        phi -= phi / (seq_t)n_tmp;
    return phi;
}

/* Compute a(n) according to the definition of the sequence. */
seq_t
a(idx_t n)
{
    if (n == 0)   /* out of range, but return 0 */
        return 0;

    /* Compute the necessary primes up to sqrt(n). */
    idx_t limit = (idx_t)floor(sqrt((long double)n)) + 1;
    ensure_primes(limit);

    seq_t phi_n = phi_for_n(n);
    seq_t result = phi_n * phi_n;      /* phi(n)^2 */
    if ((n & 1u) == 0)                 /* n even: multiply by 2 */
        result *= 2;
    return result;
}
