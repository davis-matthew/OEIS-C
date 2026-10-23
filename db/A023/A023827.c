/*
 * A023827  Sum of exponents in prime-power factorization of C(4n,n-1).
 * https://oeis.org/A023827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A004331(n)) or equivalently: a(n) = A022559(4n) –
 *            A022559(3n+1) – A022559(n-1)
 * Range:     u8 n <= 753, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static storage for prime tables ---------- */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t    prime_count  = 0;      /* number of primes stored */
static idx_t     prime_limit  = 0;      /* largest prime needed so far */

/* ---------- helper: build primes up to 'limit' (inclusive) ---------- */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free old list if it exists */
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
    }

    /* allocate boolean array for sieve */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_comp)
        return; /* out of memory – caller will handle failure */

    /* allocate primes array big enough for all primes up to limit
       (upper bound <= limit) */
    prime_list = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!prime_list) {
        free(is_comp);
        return;
    }
    prime_count = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            /* i is prime */
            prime_list[prime_count++] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    prime_limit = limit;
}

/* ---------- compute A022559(m): total exponent sum in m! ---------- */
static seq_t a022559(idx_t m)
{
    if (m == 0)
        return 0;

    unsigned long long total = 0;

    /* ensure prime list covers p <= m */
    if (prime_limit < m)
        ensure_primes(m);

    for (size_t i = 0; i < prime_count && prime_list[i] <= m; ++i) {
        idx_t p = prime_list[i];
        idx_t t = m;
        while (t) {
            t /= p;
            total += t;
        }
    }

    return (seq_t)total;
}

/* ---------- the requested sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined below offset, but safe */

    idx_t four_n = 4 * n;
    idx_t three_n_plus1 = 3 * n + 1;
    idx_t n_minus1 = n - 1;

    /* build primes up to maximum needed argument (4n) */
    ensure_primes(four_n);

    seq_t res = a022559(four_n);
    res -= a022559(three_n_plus1);
    res -= a022559(n_minus1);
    return res;
}
