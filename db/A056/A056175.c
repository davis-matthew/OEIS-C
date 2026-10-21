/*
 * A056175  Number of nonunitary prime divisors of the central binomial
 *          coefficient C(n, floor(n/2)) (A001405).
 * https://oeis.org/A056175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime divisors of the central binomial
 *            coefficient with exponent at least 2; i.e. the number of primes
 *            p for which v_p( C(n, floor(n/2)) ) >= 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t* prime_list = NULL;      /* primes up to current limit */
static size_t prime_count = 0;         /* number of primes stored */
static idx_t prime_limit = 0;          /* largest prime index checked */

/* ensure that prime_list contains all primes <= limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate a temporary array for sieve */
    size_t size = (size_t)limit + 1;
    bool *is_comp = (bool*)malloc(size);
    if (is_comp == NULL) exit(1);
    for (size_t i = 0; i < size; ++i) is_comp[i] = false;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
        if (!is_comp[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[(size_t)j] = true;

    /* Count primes to resize list if needed */
    size_t new_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i]) ++new_count;

    if (new_count > prime_count)
        prime_list = (idx_t*)realloc(prime_list, new_count * sizeof(idx_t));

    /* Fill in the list */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i])
            prime_list[idx++] = i;
    prime_count = new_count;
    prime_limit = limit;

    free(is_comp);
}

/* Legendre's formula: exponent of p in n! */
static idx_t exp_in_factorial(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n >= 1 */

    /* Guarantee prime list up to n */
    ensure_primes(n);

    idx_t k = n / 2;        /* floor(n/2) */
    seq_t count = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > n) break;
        idx_t exp = exp_in_factorial(n, p)
                - exp_in_factorial(k, p)
                - exp_in_factorial(n - k, p);
        if (exp >= 2) ++count;
    }
    return count;
}
