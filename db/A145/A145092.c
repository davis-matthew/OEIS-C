/*
 * A145092  Indices of the prime numbers after mapping the positive integers
 *          onto successive rings of length {1, 5, 30, 180, 1080, ...} and
 *          assigning index 0 to the smallest value in each.
 * https://oeis.org/A145092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = index of the nth prime within its ring in the
 *            successive‑ring mapping {1,5,30,180,1080,…}. The rings have
 *            lengths L₁=1, L₂=5, and L_k = 6·L_{k-1} for k≥3. Index 0 is
 *            assigned to the smallest value in each ring
 * Range:     u8 n <= 47, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

/* Static storage for the prime table. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;         /* number of primes stored */

/* Generate all primes up to (and including) limit using a simple sieve. */
static void generate_primes_upto_uint64(uint64_t limit)
{
    size_t size = (size_t)limit + 1;
    bool *is_composite = (bool *)malloc(size);
    if (!is_composite) return;
    memset(is_composite, 0, size);

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) {
        free(is_composite);
        return;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) new_list[idx++] = i;

    free(is_composite);
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
}

/* Ensure that we have at least 'needed' primes in prime_list. */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed) return;

    /* Heuristic upper bound: 20*n + 100 is safe for n up to ~1e5. */
    uint64_t limit = (uint64_t)needed * 20 + 100;
    if (limit < 100) limit = 100;

    generate_primes_upto_uint64(limit);

    /* If still not enough (unlikely), double the limit and repeat. */
    while (prime_count < needed) {
        limit = prime_count * 2 + 1000;
        generate_primes_upto_uint64(limit);
    }
}

/* Return the nth prime (1‑based). */
static seq_t nth_prime(idx_t n)
{
    if (n == 0) return 0;        /* defensive; not used for n>=1 */
    ensure_primes((size_t)n);
    return (seq_t)prime_list[n - 1];
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    seq_t p = nth_prime(n);

    /* Start from ring 2: first element 2 with length 5. */
    uint64_t start = 2;
    uint64_t len   = 5;

    while (p > (seq_t)(start + len - 1)) {
        start += len;   /* move to next ring */
        len   *= 6;     /* subsequent ring lengths multiply by 6 */
    }

    return p - (seq_t)start;   /* index within the ring */
}
