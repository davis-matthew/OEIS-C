/*
 * A102566  a(n) = {minimal k such that f^k(prime(n)) = 1} where f(m) =
 *          (m+1)/2^r, 2^r is the highest power of two dividing m+1.
 * https://oeis.org/A102566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal k such that f^k(prime(n)) = 1, where f(m) = (m +
 *            1) / 2^r and r is the highest exponent of 2 dividing (m + 1)
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: apply the function f to a value */
static seq_t f_val(seq_t x) {
    seq_t v = x + 1;
    while ((v & 1) == 0) v >>= 1;
    return v;
}

/* Sieve data structures */
static bool *is_prime_arr      = NULL;
static seq_t *prime_list       = NULL;
static idx_t  prime_count      = 0;
static idx_t  sieve_limit      = 0;

/* Build a sieve up to 'limit' and fill the prime list */
static void build_sieve(idx_t limit) {
    /* Allocate and initialize the prime bitmap */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return; /* allocation failed – rely on caller to handle */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p) is_prime[m] = false;
        }
    }

    /* Count primes and allocate list */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    seq_t *p_list = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!p_list) {
        free(is_prime);
        return;
    }
    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) p_list[idx++] = (seq_t)i;

    /* Replace old structures */
    free(is_prime_arr);
    free(prime_list);
    is_prime_arr = is_prime;
    prime_list   = p_list;
    prime_count  = cnt;
    sieve_limit  = limit;
}

/* Ensure we have at least 'needed' primes, expanding the sieve if necessary */
static void ensure_primes(idx_t needed) {
    if (prime_count >= needed) return;
    /* Increase limit until we have enough primes. Start with a reasonable guess. */
    idx_t new_limit = (sieve_limit == 0) ? 2000 : sieve_limit * 2;
    while (prime_count < needed) {
        build_sieve(new_limit);
        if (prime_count >= needed) break;
        new_limit *= 2;
    }
}

/* External function required by the benchmark */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, but return 0 safely */
    ensure_primes(n);
    seq_t p = prime_list[n - 1];
    seq_t steps = 0;
    while (p != 1) {
        p = f_val(p);
        ++steps;
    }
    return steps;
}
