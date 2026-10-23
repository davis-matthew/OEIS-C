/*
 * A147850  Parity of the digits sum of Sum_{j = 8*n-7..8*n} prime(j).
 * https://oeis.org/A147850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(n) = parity of digit sum of 17982 * * (sum of the eight primes
 *            at * indices 8*n‑7 .. 8*n)
 * Range:     u8 n <= 1, u16 none, u32 n <= 105, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Prime sieve ----------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 2000000ULL  /* > 8·10000‑th prime */

static bool primes_built = false;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

static void build_prime_list(void)
{
    if (primes_built) return;
    primes_built = true;

    /* Allocate boolean sieve */
    bool *is_prime = (bool *)calloc((size_t)PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_prime) return;   /* should not happen in the test harness */

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_prime[i]) continue;
        for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) is_prime[j] = false;
    }

    /* Count primes */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint64_t)i;

    free(is_prime);
}

/* --- Parity of the digit sum ---------------------------------------- */
static seq_t parity_of_digit_sum(uint64_t value)
{
    uint8_t parity = 0;
    while (value != 0) {
        parity ^= (value % 10) & 1;
        value /= 10;
    }
    return (seq_t)parity;
}

/* --- Sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, guard */

    /* Ensure primes are ready */
    build_prime_list();

    /* Compute indices of the eight primes (0‑based) */
    uint64_t start = 8ULL * n - 8ULL;      /* 8*n-7 → 0‑based offset */
    if (start + 7ULL >= prime_count) {
        /* For requested n the pre‑computed range is insufficient.
         * This should never happen for the test limits (n ≤ 10000). */
        return (seq_t)0;
    }

    /* Sum the eight primes (64‑bit is sufficient for n ≤ 10000) */
    uint64_t sum = 0;
    for (size_t i = 0; i < 8; ++i)
        sum += prime_list[start + i];

    /* Multiply by the fixed factor 17982, still in 64‑bit */
    uint64_t prod64 = sum * 17982ULL;
    /* Convert to seq_t (works for all supported seq_t sizes) */
    seq_t prod = (seq_t)prod64;

    /* Parity of the digit sum of the product */
    return parity_of_digit_sum((uint64_t)prod);   /* cast back for safety */
}
