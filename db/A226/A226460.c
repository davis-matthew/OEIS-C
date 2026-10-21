/*
 * A226460  Let m! have prime factorization Product (p_j^e_j); a(n) = number
 *          of distinct prime factors p_j such that e_j = n has no solution
 *          for any m!.
 * https://oeis.org/A226460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = number of primes p ≤ n such that there is no
 * integer m with Legendre(p, m) = n (i.e. p^n never divides any factorial).
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the primes up to the highest requested limit */
static uint64_t *prime_list = NULL;
static idx_t   prime_count = 0;   /* number of primes stored */
static idx_t   prime_limit = 0;   /* upper bound already sieved */

/* ----------------------------------------------------------------------- */
/* sieve_to: fill prime_list with all primes ≤ limit.                         */
static void sieve_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;                            /* already available */

    bool *is_prime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);               /* memory allocation failed */

    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    idx_t npr = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++npr;

    /* allocate / reallocate prime_list */
    uint64_t *tmp = (uint64_t*)realloc(prime_list, npr * sizeof(uint64_t));
    if (!tmp) exit(1);
    prime_list = tmp;

    /* fill prime_list */
    idx_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[k++] = i;

    prime_count = npr;
    prime_limit = limit;

    free(is_prime);
}

/* ----------------------------------------------------------------------- */
/* legendre: compute e_p(m!) = sum_{i>=1} floor(m / p^i)                  */
static idx_t legendre(idx_t m, uint64_t p)
{
    idx_t sum = 0;
    while (m) {
        m /= p;
        sum += m;
    }
    return sum;
}

/* ----------------------------------------------------------------------- */
/* exponent_possible: does there exist m with e_p(m!) = n?                   */
static bool exponent_possible(idx_t n, uint64_t p)
{
    if (p > n)                 /* for p > n, choose m = n*p; e_p = n */
        return true;
    if (n == 0)                /* exponent 0 is always possible */
        return true;

    idx_t low  = 0;
    idx_t high = n * p;                /* high is safe: e_p(high) >= n for p>=2 */

    /* binary search minimal m with e_p(m!) >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (legendre(mid, p) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return legendre(low, p) == n;
}

/* ----------------------------------------------------------------------- */
/* Main sequence function                                                    */
seq_t a(idx_t n)
{
    if (n < 2)                 /* a(0) = a(1) = 0 */
        return (seq_t)0;

    sieve_to(n);                /* ensure primes up to n are available */

    idx_t cnt = 0;
    for (idx_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (!exponent_possible(n, p))
            ++cnt;
    }
    return (seq_t)cnt;
}
