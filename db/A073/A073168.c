/*
 * A073168  a(n) = A007821(n) - A022449(n).
 * https://oeis.org/A073168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007821(n) - A022449(n)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t *prime_vals = NULL;
static size_t  prime_len  = 0;
static idx_t *composite_vals = NULL;
static size_t  composite_len = 0;
static int    initialized = 0;

/* Build a sieve up to LIMIT and record primes and composites */
static void init_cache(void)
{
    if (initialized) return;
    const idx_t LIMIT = (idx_t)2000000;          /* enough for n ≤ ~10⁴ */

    unsigned char *is_prime = (unsigned char *)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!is_prime) return;
    memset(is_prime, 1, (LIMIT + 1) * sizeof(unsigned char));
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= LIMIT; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = 0;

    /* Count primes and composites (composites start from 4) */
    prime_len = 0;
    composite_len = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) ++prime_len;
        else if (i >= 4) ++composite_len;
    }

    prime_vals = (idx_t *)malloc(prime_len   * sizeof(idx_t));
    composite_vals = (idx_t *)malloc(composite_len * sizeof(idx_t));
    if (!prime_vals || !composite_vals) {
        free(is_prime);
        free(prime_vals);
        free(composite_vals);
        return;
    }

    /* Fill arrays */
    size_t pi = 0, ci = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i])
            prime_vals[pi++] = i;
        else if (i >= 4)
            composite_vals[ci++] = i;
    }

    free(is_prime);
    initialized = 1;
}

/* Return the nth prime (n ≥ 1) */
static inline seq_t prime_of(idx_t n)
{
    return (seq_t)prime_vals[n - 1];
}

/* Return the nth composite (n ≥ 1) */
static inline seq_t composite_of(idx_t n)
{
    return (seq_t)composite_vals[n - 1];
}

/* Sequence A073168 */
seq_t a(idx_t n)
{
    init_cache();

    if (n < 1) return (seq_t)0;

    /* composite(n) → index for prime() */
    idx_t comp_n_index = composite_vals[n - 1];
    /* prime(n)    → index for composite() */
    idx_t prime_n_index = prime_vals[n - 1];

    seq_t val1 = prime_of(comp_n_index);   /* prime(composite(n)) */
    seq_t val2 = composite_of(prime_n_index); /* composite(prime(n)) */

    return val1 - val2;
}
