/*
 * A127339  Numbers that are the sum of 12 consecutive primes.
 * https://oeis.org/A127339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of 12 consecutive primes starting at prime(n)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for prime numbers */
static uint32_t *prime_list = NULL;
static size_t     prime_count = 0;
static uint32_t   last_checked = 1;   /* candidate to test next */

/* Ensure that prime_list contains at least 'needed' primes. */
static void ensure_prime_list(size_t needed)
{
    if (prime_count >= needed)
        return;

    /* Grow capacity to at least 'needed' */
    size_t new_capacity = prime_count > 0 ? prime_count * 2 : 16;
    while (new_capacity < needed)
        new_capacity <<= 1;

    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_capacity * sizeof(uint32_t));
    if (!tmp)
        abort();   /* out of memory */
    prime_list = tmp;

    /* Simple incremental sieve (trial division) */
    while (prime_count < needed) {
        uint32_t candidate = ++last_checked;
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            prime_list[prime_count++] = candidate;
    }
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Need primes from index n to n+11 (1‑based) */
    size_t need = (size_t)n + 11;
    ensure_prime_list(need);

    seq_t sum = (seq_t)0;
    size_t start = (size_t)(n - 1);
    for (size_t i = 0; i < 12; ++i)
        sum += (seq_t)prime_list[start + i];
    return sum;
}
