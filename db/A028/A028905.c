/*
 * A028905  Arrange digits of primes in ascending order.
 * https://oeis.org/A028905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sort(prime(n)) (OEIS A028905)
 * Range:     u8 n <= 54, u16 n <= 6646, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached list of primes used to compute prime(n) by trial division. */
static uint32_t next_candidate = 2;
static uint32_t *prime_list = NULL;
static size_t prime_count     = 0;
static size_t prime_capacity  = 0;

/* Ensure that prime_list contains at least 'needed' primes. */
static void ensure_primes(size_t needed)
{
    while (prime_count < needed) {
        uint32_t candidate = next_candidate++;
        /* Skip even numbers > 2 */
        if (candidate > 2 && (candidate & 1U) == 0U)
            continue;

        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;

        /* Grow the prime array if needed */
        if (prime_count == prime_capacity) {
            size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
            uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
            if (!tmp) abort();    /* allocation failed */
            prime_list   = tmp;
            prime_capacity = new_cap;
        }
        prime_list[prime_count++] = candidate;
    }
}

/* Return the nth term of OEIS A028905 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0, but return 0 safely */

    /* Produce prime(n) */
    ensure_primes((size_t)n);
    uint32_t p = prime_list[(size_t)n - 1];

    /* Count decimal digits of p */
    unsigned char digit_cnt[10] = {0};
    uint32_t tmp = p;
    if (tmp == 0) digit_cnt[0] = 1;  /* should not happen for a prime */
    while (tmp > 0) {
        unsigned d = tmp % 10;
        ++digit_cnt[d];
        tmp /= 10;
    }

    /* Build the sorted‑digit result, discarding leading zeros */
    seq_t result = 0;
    bool leading = true;
    for (unsigned d = 0; d <= 9; ++d) {
        for (unsigned i = 0; i < digit_cnt[d]; ++i) {
            if (leading && d == 0)
                continue;  /* skip leading zeros */
            leading = false;
            result = result * 10 + (seq_t)d;
        }
    }
    return result;
}
