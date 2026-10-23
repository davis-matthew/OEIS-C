/*
 * A157713  a(n) = (2*n+1)!*(2*n-2)!/((n-1)!*(n!)^2*6).
 * https://oeis.org/A157713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)! * (2*n-2)! / ((n-1)! * (n!)^2 * 6)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Applicable for n >= 1.  Uses prime factorisation to keep each step tiny. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global state for a simple prime sieve */
static int  *global_primes = NULL;     /* array of primes */
static size_t global_primes_count = 0; /* number of primes stored */
static size_t global_primes_limit = 0; /* largest number for which sieve has been run */

/* ------------------------------------------------------------------ */
/* Simple Eratosthenes sieve: grow the table to contain all primes ≤ limit */
static void ensure_primes(size_t limit)
{
    if (limit <= global_primes_limit)
        return;                          /* already sufficient */

    size_t sz = limit + 1;
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime)
        return;                          /* OOM – caller will die gracefully */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    /* Count primes first */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    if (cnt > global_primes_count) {
        int *tmp = (int *)realloc(global_primes, cnt * sizeof(int));
        global_primes = tmp;
        global_primes_count = cnt;
    }

    /* Fill array with primes */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            global_primes[idx++] = (int)i;

    global_primes_limit = limit;
    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* exponent of prime p in m! (Legendre's formula) */
static idx_t fact_exp(idx_t m, int p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* ------------------------------------------------------------------ */
/* fast binary exponentiation for arbitrary seq_t type */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* required external function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Create all primes needed for the factorisation */
    size_t limit = 2 * (size_t)n + 1;
    ensure_primes(limit);

    seq_t result = (seq_t)1;

    /* For every prime p ≤ limit, add its net exponent in the fraction */
    for (size_t i = 0; i < global_primes_count && global_primes[i] <= (int)limit; ++i) {
        int p = global_primes[i];
        idx_t exp = fact_exp(2 * n + 1, p)    /* (2n+1)!   */
                 + fact_exp(2 * n - 2, p)    /* (2n-2)!   */
                 - fact_exp(n - 1, p)        /* (n-1)!    */
                 - 2 * fact_exp(n, p);       /* (n!)^2    */

        /* division by 6 = 2 * 3 */
        if (p == 2 || p == 3)
            exp -= 1;

        if (exp == 0)
            continue;

        seq_t pw = pow_seq_local((seq_t)p, exp);
        result = result * pw;
    }

    return result;
}
