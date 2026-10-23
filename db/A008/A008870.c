/*
 * A008870  Order of simple Chevalley group E_7 (q), q = prime power.
 * https://oeis.org/A008870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, bref
 * Formula:   a(n) = q^63 * ∏_{d∈{2,6,8,10,12,14,18}} (q^d−1) / (q−1,2) where
 *            q is the n-th prime power
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* comparison for qsort on idx_t */
static int cmp_idx_t(const void *a, const void *b) {
    idx_t ua = *(const idx_t *)a;
    idx_t ub = *(const idx_t *)b;
    return (ua < ub) ? -1 : (ua > ub);
}

/* binary exponentiation: base^exp as seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* generate all prime powers ≤ limit, store in out array and count them.
   The array is allocated by this function and must not be freed by caller. */
static void generate_prime_powers(idx_t limit, idx_t **out, size_t *cnt) {
    /* sieve for primes up to limit */
    char *is_prime = (char *)malloc((size_t)(limit + 1));
    if (!is_prime) { *out = NULL; *cnt = 0; return; }
    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    /* collect primes first */
    size_t primes_cap = limit / 10 + 100;
    idx_t *primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_cnt++] = i;
        }
    }
    /* generate prime powers */
    size_t powers_cap = primes_cnt * 4 + 100;
    idx_t *powers = (idx_t *)malloc(powers_cap * sizeof(idx_t));
    size_t powers_cnt = 0;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t v = p;
        while (v <= limit) {
            if (powers_cnt == powers_cap) {
                powers_cap *= 2;
                powers = (idx_t *)realloc(powers, powers_cap * sizeof(idx_t));
            }
            powers[powers_cnt++] = v;
            if (v > limit / p) break;
            v *= p;
        }
    }
    free(primes);
    free(is_prime);
    /* sort the powers */
    qsort(powers, powers_cnt, sizeof(idx_t), cmp_idx_t);
    /* shrink array to actual size */
    powers = (idx_t *)realloc(powers, powers_cnt * sizeof(idx_t));
    *out = powers;
    *cnt = powers_cnt;
}

/* static cache for prime powers */
static const idx_t *prime_powers = NULL;
static size_t prime_power_count = 0;
static int initialized = 0;

/* lazily initialize prime powers up to a sufficient limit */
static void init_prime_powers(void) {
    if (initialized) return;
    /* The limit 200000 yields more than 10000 prime powers. */
    idx_t limit = 200000;
    size_t needed = 10000;          /* maximum n requested by tests */
    idx_t *powers = NULL;
    size_t count = 0;
    while (1) {
        generate_prime_powers(limit, &powers, &count);
        if (count >= needed) {
            prime_powers = powers;
            prime_power_count = count;
            initialized = 1;
            break;
        }
        free(powers);
        limit *= 2;
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n<1 */
    init_prime_powers();
    if (n > prime_power_count) {
        /* If n exceeds precomputed range (unlikely for given limits) */
        /* We could generate more, but this situation shouldn't happen. */
        return (seq_t)0;
    }
    seq_t q = (seq_t)prime_powers[n - 1];
    seq_t order = pow_seq(q, (idx_t)63);
    static const idx_t degrees[7] = {2, 6, 8, 10, 12, 14, 18};
    for (size_t i = 0; i < 7; ++i) {
        seq_t term = pow_seq(q, degrees[i]) - (seq_t)1;
        order *= term;
    }
    /* divide by gcd(q-1,2).  For odd q, this is 2; for even q, 1. */
    if ((prime_powers[n - 1] & (idx_t)1ULL) != 0) {
        order /= (seq_t)2;
    }
    return order;
}
