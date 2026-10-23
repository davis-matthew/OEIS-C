/*
 * A166351  a(n) = (6*n)!/(3*n)!.
 * https://oeis.org/A166351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n)!/(3*n)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count exponent of prime p in m! */
static idx_t exp_in_fact(idx_t m, idx_t p) {
    idx_t e = 0;
    idx_t pp = p;
    while (pp <= m) {
        e += m / pp;
        if (pp > m / p) break;  /* prevent overflow */
        pp *= p;
    }
    return e;
}

/* Fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* Cached prime list up to the largest needed limit */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;
static idx_t    prime_limit = 0;

/* Ensure primes are computed up to 'limit' */
static void ensure_primes(idx_t limit) {
    if (limit <= prime_limit) return;

    free(prime_list);
    prime_limit = limit;
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool*)malloc(sz * sizeof(bool));
    if (!is_prime) exit(1);

    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    if (sz > 0) is_prime[0] = false;
    if (sz > 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);
    prime_count = cnt;
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint64_t)i;

    free(is_prime);
}

/*
 * seq_t a(idx_t n)
 *
 * Computes a(n) = (6*n)!/(3*n)!
 * The result is returned as type seq_t (unsigned integer or
 * arbitrary‑precision integer class).
 */
seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t limit = 6 * n;
    ensure_primes(limit);

    seq_t result = 1;
    idx_t big3 = 3 * n;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > limit) break;

        idx_t e = exp_in_fact(limit, p) - exp_in_fact(big3, p);
        if (e == 0) continue;

        seq_t powp = pow_seq((seq_t)p, e);
        result *= powp;
    }

    return result;
}
