/*
 * A239681  Primality of Mersenne number 2^prime(n)-1.
 * https://oeis.org/A239681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the n-th prime is an exponent of a known Mersenne
 *            prime, else 0
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* static cache of primes */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;

/* the list of exponents of known Mersenne primes ≤ 104 729 (the 10 000‑th prime) */
static const idx_t known_exponents[] = {
    2, 3, 5, 7, 13, 17, 19, 31, 61, 89,
    107, 127, 521, 607, 1279, 2203, 2281,
    3217, 4253, 4423, 9689, 9941, 11213,
    19937, 21701, 23209, 44497, 86243
};
static const size_t known_exponent_count =
    sizeof(known_exponents) / sizeof(known_exponents[0]);

/* binary search in the known_exponents array */
static bool is_known_exponent(idx_t p)
{
    idx_t low = 0, high = known_exponent_count - 1;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t val = known_exponents[mid];
        if (val == p) return true;
        if (val < p) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

/* build a sieve of Eratosthenes up to a fixed limit and store all primes */
static void init_prime_sieve(void)
{
    const idx_t LIMIT = 200000;          /* > 10 000-th prime (≈104 729) */
    bool *is_comp = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_comp) return;
    for (idx_t i = 0; i <= LIMIT; ++i) is_comp[i] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) {
        free(is_comp);
        return;
    }
    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) prime_list[pos++] = (uint64_t)i;
    }
    prime_count = cnt;
    free(is_comp);
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset */

    if (!prime_list) init_prime_sieve();
    if (n > prime_count) return 0;   /* out of range defensive */

    idx_t p = prime_list[n - 1];   /* nth prime, 1‑based */
    return (seq_t)(is_known_exponent(p) ? 1 : 0);
}
