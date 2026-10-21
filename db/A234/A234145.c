/*
 * A234145  a(n) = denominator of sum_(k=1..n) 1/(2*k-1)^n.
 * https://oeis.org/A234145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = product of all odd primes p ≤ 2*n-1 of p^(n *
 *            floor(log_p(2*n-1))). The product is over primes, so the result
 *            is the least common multiple of (2k-1)^n for k = 1..n, i.e. the
 *            denominator of sum_{k=1..n} 1/(2k-1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* fast exponentiation: base^exp for seq_t base, exp as idx_t */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* dynamic list of odd primes up to a current maximum limit */
static idx_t *prime_list   = NULL;
static size_t prime_count  = 0;
static idx_t  max_limit    = 0;

/* build list of odd primes ≤ LIMIT */
static void build_primes(idx_t LIMIT)
{
    bool *composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!composite)
        return;               /* unlikely, but guard */

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                composite[j] = true;
        }
    }

    /* count odd primes */
    size_t cnt = 0;
    for (idx_t i = 3; i <= LIMIT; i += 2)
        if (!composite[i])
            ++cnt;

    /* allocate and store odd primes */
    free(prime_list);
    prime_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!prime_list) {
        free(composite);
        return;
    }

    size_t pos = 0;
    for (idx_t i = 3; i <= LIMIT; i += 2) {
        if (!composite[i])
            prime_list[pos++] = i;
    }
    prime_count = cnt;
    max_limit   = LIMIT;

    free(composite);
}

/* ensure that prime list covers odd primes up to LIMIT */
static void ensure_primes(idx_t LIMIT)
{
    if (max_limit >= LIMIT)
        return;
    build_primes(LIMIT);
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return 1;

    idx_t limit_val = 2 * n - 1;     /* largest odd denominator */
    ensure_primes(limit_val);

    seq_t prod = 1;

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = prime_list[pi];

        /* maximum exponent e of p in any odd integer ≤ limit_val */
        idx_t power = 1;
        int   e     = 0;
        while (power <= limit_val / p) {
            power *= p;
            ++e;
        }

        if (e == 0)            /* p does not divide any odd number ≤ limit_val */
            continue;

        idx_t exp = n * (idx_t)e;
        prod *= pow_seq((seq_t)p, exp);
    }

    return prod;
}
