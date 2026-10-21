/*
 * A063955  Sum of the unitary prime divisors of n!.
 * https://oeis.org/A063955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of primes p such that the exponent of p in n! is
 *            exactly 1
 * Range:     u8 n <= 52, u16 n <= 1086, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = sum of primes p with n/2 < p ≤ n                       */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build environment */
extern seq_t a(idx_t n);

/* ---------- globals for prime table ---------- */
static idx_t  max_computed = 0;      /* largest n for which primes are known   */
static uint32_t *prime_list = NULL;  /* array of primes up to max_computed     */
static size_t   prime_count = 0;     /* number of primes stored                */

/* recompute primes up to n (full sieve) */
static void recompute_primes(idx_t n)
{
    if (n <= max_computed)           /* nothing to do if already enough */
        return;

    /* allocate composite flags */
    bool *is_composite = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_composite)
        abort();

    /* simple Eratosthenes sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (!is_composite[(size_t)i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_composite[(size_t)j] = true;
        }
    }

    /* count primes to know the needed storage */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!is_composite[(size_t)i])
            ++cnt;

    /* allocate space for the primes */
    uint32_t *new_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_list)
        abort();

    /* collect primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_composite[(size_t)i])
            new_list[pos++] = (uint32_t)i;
    }

    /* replace the old table */
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;

    /* clean up */
    free(is_composite);
    max_computed = n;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1)                      /* a(1) = 0 by definition */
        return (seq_t)0;

    /* make sure primes up to n are ready */
    recompute_primes(n);

    /* find the first prime strictly greater than n/2 */
    idx_t half = n / 2;
    size_t start = 0;
    while (start < prime_count && prime_list[start] <= (uint32_t)half)
        ++start;

    /* sum primes from start to the end of the list */
    seq_t sum = (seq_t)0;
    for (size_t i = start; i < prime_count; ++i)
        sum += (seq_t)prime_list[i];

    return sum;
}
