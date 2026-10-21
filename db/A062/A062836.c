/*
 * A062836  Floor[ Sum_{k=1..n,p(k)^2} / Sum_{k=1..n,p(k)-1} ].
 * https://oeis.org/A062836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{k=1..n} p(k)^2 / Σ_{k=1..n} (p(k)-1) ), where
 *            p(k) is the k‑th prime
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static state for primes and cumulative sums. */
static int initialized = 0;
static idx_t *prime_list  = NULL;   /* 1‑based primes: prime_list[1] = 2          */
static seq_t *cum_sum     = NULL;   /* Σ p(k)                                     */
static seq_t *cum_sq_sum  = NULL;   /* Σ p(k)^2                                   */
static idx_t prime_count  = 0;

/* Sieve of Eratosthenes up to a fixed upper bound sufficient for the 10 000‑th prime. */
static void init_primes(void)
{
    const idx_t LIMIT = 110000;          /* > 10 000th prime (104 729) */

    /* Allocate boolean array of primes. */
    char *is_prime = (char*)calloc((size_t)LIMIT + 1, sizeof(char));
    if (!is_prime) return;              /* memory failure – nothing returned */

    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;

    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }

    /* Count primes to size arrays. */
    for (idx_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++prime_count;

    prime_list  = (idx_t*)malloc((size_t)(prime_count + 1) * sizeof(idx_t));
    cum_sum     = (seq_t*)malloc((size_t)(prime_count + 1) * sizeof(seq_t));
    cum_sq_sum  = (seq_t*)malloc((size_t)(prime_count + 1) * sizeof(seq_t));
    if (!prime_list || !cum_sum || !cum_sq_sum) {
        free(is_prime);
        if (prime_list)  free(prime_list);
        if (cum_sum)     free(cum_sum);
        if (cum_sq_sum)  free(cum_sq_sum);
        return;
    }

    cum_sum[0]    = (seq_t)0;
    cum_sq_sum[0] = (seq_t)0;
    idx_t idx = 1;
    for (idx_t i = 2; i <= LIMIT && idx <= prime_count; ++i) {
        if (is_prime[i]) {
            prime_list[idx] = i;
            cum_sum[idx]     = cum_sum[idx-1] + (seq_t)i;
            cum_sq_sum[idx]  = cum_sq_sum[idx-1] + (seq_t)i * (seq_t)i;
            ++idx;
        }
    }
    free(is_prime);
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init_primes();
    if (!initialized) return (seq_t)0;   /* cannot initialise – safe default */

    if (n == 0) return (seq_t)0;         /* defined only for n ≥ 1, guard anyway */

    if (n > prime_count) n = prime_count;   /* safety clamp */

    seq_t denom = cum_sum[n] - (seq_t)n;    /* Σ p(k) – n  = Σ (p(k) – 1) */
    seq_t numer = cum_sq_sum[n];            /* Σ p(k)^2      */

    return numer / denom;                  /* integer division → floor */
}
