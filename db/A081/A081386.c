/*
 * A081386  Number of unitary prime divisors of central binomial coefficient,
 *          C(2n,n) = A000984(n), i.e., number of those prime factors in
 *          C(2n,n), whose exponent equals one.
 * https://oeis.org/A081386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unitary prime divisors of C(2n,n) (i.e., primes
 *            p such that exponent of p in C(2n,n) equals 1)
 * Range:     u8 n <= 1311, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Forward declaration of helper to avoid compiler warnings */
static void ensure_primes(idx_t up_to);

/* Legendre exponent of p in n! */
static inline idx_t legendre_factorial(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n)
    {
        n /= p;
        exp += n;
    }
    return exp;
}

/* Global storage for primes */
static uint32_t *primes = NULL;      /* array of primes */
static size_t prime_cnt = 0;       /* number of primes */
static idx_t prime_limit = 0;      /* highest prime that has been sieved */
static size_t primes_cap = 0;      /* allocated capacity for primes */

/* Allocate/extend sieve up to `up_to` and fill primes array */
static void ensure_primes(idx_t up_to)
{
    if (up_to <= prime_limit) return;          /* nothing to do */

    /* New limit **/
    idx_t limit = up_to;
    size_t size = (size_t)limit + 1;

    /* Simple sieve: boolean array */
    char *is_comp = (char*)calloc(size, 1);
    if (!is_comp) return;      /* allocation failed, just skip */

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Allocate / reallocate primes buffer */
    if (cnt > primes_cap)
    {
        uint32_t *new_buf = (uint32_t*)realloc(primes, sizeof(uint32_t) * cnt);
        if (!new_buf)
        {
            free(is_comp);
            return;  /* allocation failed */
        }
        primes = new_buf;
        primes_cap = cnt;
    }

    /* Fill primes array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;

    prime_cnt = cnt;
    prime_limit = limit;
    free(is_comp);
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* Ensure we have all primes up to 2*n */
    ensure_primes(n + n);

    idx_t two_n = n + n;
    idx_t count = 0;

    /* Iterate over primes up to 2*n */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = primes[i];
        if (p > two_n) break;

        idx_t exp = legendre_factorial(two_n, p) - 2 * legendre_factorial(n, p);
        if (exp == 1) ++count;
    }

    return (seq_t)count;
}
