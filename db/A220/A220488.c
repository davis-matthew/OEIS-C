/*
 * A220488  Primes that have both prime digits (2,3,5,7) and nonprime digits
 *          (1,4,6,8,9), without digits "0".
 * https://oeis.org/A220488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 994 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes that contain at least one prime digit (2,3,5,7)
   and at least one non‑prime digit (1,4,6,8,9) with no digit 0,
   with on‑demand search and caching. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime_seq.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool has_required_digits(idx_t x)
{
    bool prime_digit   = false;
    bool nonprime_digit = false;

    while (x > 0) {
        uint32_t d = (uint32_t)(x % 10);
        if (d == 0) return false;           /* reject digits 0 */
        if (d == 2 || d == 3 || d == 5 || d == 7)
            prime_digit = true;
        else
            nonprime_digit = true;
        x /= 10;
    }
    return prime_digit && nonprime_digit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, safe fallback */

    /* static cache of previously found terms */
    static seq_t *cache         = NULL;
    static size_t cache_len     = 0;   /* number of terms stored */
    static size_t cache_cap     = 0;   /* allocated capacity */
    static idx_t  last_checked  = 2;   /* next integer to test */

    while (cache_len < (size_t)n) {
        /* test the next integer */
        if (is_prime_seq((uint64_t)last_checked) &&
            has_required_digits(last_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL)
                    abort();
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
        last_checked++;
    }

    return cache[n - 1];
}
