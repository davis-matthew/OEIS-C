/*
 * A089241  Even numbers k such that k/2 - 1 is prime.
 * https://oeis.org/A089241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*prime(n) + 2 (prime(n) = n-th prime, 1‑based)
 * Range:     u8 n <= 30, u16 n <= 3512, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Prime cache -------------------------------------------------------------- */
static seq_t *prime_cache = NULL;   /* array of primes (1‑based for convenience) */
static size_t prime_cap   = 0;      /* allocated capacity */
static size_t prime_cnt   = 0;      /* number of primes stored */

/* Ensure the cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_cnt) return;           /* enough primes already */

    /* Grow the cache array if needed. */
    if (prime_cap < (size_t)n) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_cache = tmp;
        prime_cap = new_cap;
    }

    /* First prime. */
    if (prime_cnt == 0) {
        prime_cache[prime_cnt++] = (seq_t)2;
    }

    /* Generate further primes by trial division using stored primes. */
    unsigned long long cand = (unsigned long long)prime_cache[prime_cnt - 1] + 1;
    while (prime_cnt < (size_t)n) {
        int is_prime = 1;                         /* assume prime */
        for (size_t i = 0; i < prime_cnt; ++i) {
            unsigned long long p = (unsigned long long)prime_cache[i];
            if (p * p > cand) break;              /* division limit reached */
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            prime_cache[prime_cnt++] = (seq_t)cand;
        }
        ++cand;
    }
}

/* Sequence function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n < 1 */

    ensure_primes(n);
    seq_t p = prime_cache[n - 1];      /* nth prime (0‑based index) */

    /* Compute 2*p + 2 using seq_t arithmetic. */
    return ((seq_t)2 * p) + (seq_t)2;
}
