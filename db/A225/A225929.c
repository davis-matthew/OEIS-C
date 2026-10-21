/*
 * A225929  Number of conjugacy classes in Chevalley group G_2(q) as q runs
 *          through the prime powers.
 * https://oeis.org/A225929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q^2 + 2q + (q mod 6 == 1 || 5 ? 9 : 8), where q is the
 *            n‑th prime power
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq_t(const void *a, const void *b) {
    seq_t aa = *(const seq_t*)a;
    seq_t bb = *(const seq_t*)b;
    return (aa < bb) ? -1 : (aa > bb) ? 1 : 0;
}

/* Static state for memoisation */
static seq_t *prime_powers = NULL;
static idx_t  prime_power_count = 0;
static bool   initialized = false;

/* Build the first N prime powers (sufficient for n up to 10000) */
static void init(void) {
    const idx_t LIMIT = 200000;           /* enough to cover 10000 terms */
    size_t lim = (size_t)LIMIT;

    /* Sieve of Eratosthenes up to LIMIT */
    char *is_prime = (char*)calloc(lim + 1, 1);
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, lim + 1);
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= lim; j += i) is_prime[j] = 0;
        }
    }

    /* Dynamic array for prime powers */
    size_t cap = 1024;
    prime_powers = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!prime_powers) exit(EXIT_FAILURE);

    /* Generate all prime powers <= LIMIT */
    for (size_t p = 2; p <= lim; ++p) {
        if (!is_prime[p]) continue;
        seq_t val = (seq_t)p;
        for (;;) {
            if (prime_power_count >= cap) {
                cap <<= 1;
                prime_powers = (seq_t*)realloc(prime_powers, cap * sizeof(seq_t));
                if (!prime_powers) exit(EXIT_FAILURE);
            }
            prime_powers[prime_power_count++] = val;
            /* Prepare next power; stop if it would exceed LIMIT */
            if (val > ((seq_t)LIMIT / (seq_t)p)) break;
            val *= (seq_t)p;
        }
    }

    /* Sort the list of prime powers */
    qsort(prime_powers, prime_power_count, sizeof(seq_t), cmp_seq_t);

    free(is_prime);
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init();

    /* n is 1‑based */
    idx_t idx = n - 1;
    if (idx >= prime_power_count) {
        /* This should not happen for the intended n range */
        return (seq_t)0;
    }

    seq_t q = prime_powers[idx];
    seq_t q2 = q * q;
    uint64_t r6 = (uint64_t)(q % (seq_t)6);
    seq_t c = (r6 == 1 || r6 == 5) ? (seq_t)9 : (seq_t)8;
    return q2 + ((q << 1) + c);
}
