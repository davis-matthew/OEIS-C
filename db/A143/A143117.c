/*
 * A143117  a(n) = the smallest positive integer that divides no more than one
 *          integer k, where n-th prime <= k <= (n+1)th prime.
 * https://oeis.org/A143117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer d ≥ 2 such that in the interval [p_n ,
 *            p_{n+1}] (p_k = k-th prime) there are at most one multiple of d.
 *            Here p_k are the consecutive primes
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Pre‑compute a sufficient number of primes (up to 200 000).
   This covers n up to at least 10000 (p_10001 ≈ 110591). */
#define SIEVE_LIMIT 200000

static uint32_t primes[SIEVE_LIMIT + 1];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void) {
    if (primes_ready) return;
    bool is_comp[SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_comp[i] = false;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i]) primes[prime_cnt++] = (uint32_t)i;

    primes_ready = true;
}

/* Return the a(n) value as defined above. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;        /* undefined for n < 1, safe fallback */
    init_primes();

    /* We need primes up to index n+1 (1‑based). */
    if (n >= prime_cnt) return (seq_t)0; /* safety check */

    idx_t p = primes[n - 1];   /* p_n */
    idx_t p_next = primes[n];  /* p_{n+1} */

    for (idx_t d = 2; ; ++d) {
        /* Count multiples of d in [p, p_next] */
        idx_t cnt = p_next / d - (p - 1) / d;
        if (cnt <= 1)
            return (seq_t)d;
    }
}
