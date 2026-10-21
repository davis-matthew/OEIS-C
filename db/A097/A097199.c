/*
 * A097199  Numbers of the form p^4 + 4^p for p prime.
 * https://oeis.org/A097199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^4 + 4^p_n where p_n is the n‑th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime handling – simple sieve up to a fixed upper bound (enough for n≤10000) */
#define MAX_PRIMES  20000          /* 10000th prime is < 120000 */
#define PRIME_MAX   150000         /* upper limit for sieve, > 10000th prime */

static uint32_t prime_cache[MAX_PRIMES];
static size_t   prime_cache_size = 0;

static void
ensure_primes(idx_t n)
{
    if (prime_cache_size >= n) return;

    /* Simple Eratosthenes sieve up to PRIME_MAX */
    bool *is_comp = (bool *)malloc(PRIME_MAX + 1);
    if (!is_comp) return; /* allocation failure – ignore */

    for (size_t i = 0; i <= PRIME_MAX; ++i) is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    for (size_t i = 2; i * i <= PRIME_MAX; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_MAX; j += i) {
                is_comp[j] = true;
            }
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= PRIME_MAX && count < MAX_PRIMES; ++i) {
        if (!is_comp[i]) {
            prime_cache[count++] = (uint32_t)i;
        }
    }
    prime_cache_size = count;
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Small integer exponentiation for seq_t */
static seq_t
pow_seq_small(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Sequence function as requested */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below offset – not used */

    ensure_primes(n);

    /* nth prime (prime_cache is 0‑based) */
    uint32_t p = prime_cache[n - 1];

    /* compute p^4 and 4^p */
    seq_t p_term = pow_seq_small((seq_t)p, 4);
    seq_t four_term = pow_seq_small((seq_t)4, (idx_t)p);

    return p_term + four_term;
}
