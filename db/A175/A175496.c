/*
 * A175496  Positive integers k where k is not squarefree but the number of
 *          divisors of k is a power of 2.
 * https://oeis.org/A175496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by brute‑force search using prime factorisation
 * Range:     u8 n <= 21, u16 n <= 5244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_power_of_two, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_power_of_two.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Test whether a number k (32/64 bit) has
 * a divisor count that is a power of two
 * and k is not square‑free. */
static bool is_candidate(uint64_t k)
{
    uint64_t n = k;
    bool has_square = false;
    seq_t tau = 1;                 /* divisor count */

    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        unsigned int e = 0;
        while (n % p == 0) {
            ++e;
            n /= p;
        }
        if (e >= 2) has_square = true;
        tau = tau * (seq_t)(e + 1);
    }
    return has_square && is_power_of_two(tau);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range – never called by the harness */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_checked = 1;   /* number just examined */

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_candidate(last_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) return 0;   /* allocation failure – should not happen */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
