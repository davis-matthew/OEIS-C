/*
 * A106902  Primes of the form 3x^2-3xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime of the form 3x^2 - 3xy + 5y^2, with x,y ≥ 0,
 *            enumerated in increasing order. The function builds a growing
 *            cache of primes of that form on demand. The test for a given
 *            prime uses a simple loop over y, computing the discriminant D =
 *            12p – 51y² and checking whether it is a perfect...
 * Range:     u8 n <= 16, u16 n <= 1633, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt_uint64, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/int_sqrt_uint64.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

static bool has_form_prime(uint64_t p)
{
    /* Search for non‑negative integer solutions (x,y) of
       3x^2 - 3xy + 5y^2 = p.  */
    for (uint64_t y = 0;; ++y) {
        uint64_t yy = y * y;
        /* D = 12p - 51 y^2.  Stop when D < 0.  */
        uint64_t D = 12 * p;
        if (51 * yy > D) break;
        D -= 51 * yy;
        uint64_t sq = int_sqrt_uint64(D);
        if (sq * sq != D) continue;
        /* Check both ± solutions for integer x.  */
        uint64_t num1 = 3 * y + sq;
        if (num1 % 6 == 0) return true;
        uint64_t num2 = 3 * y >= sq ? 3 * y - sq : 0;
        if (num2 % 6 == 0) return true;
    }
    return false;
}

void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < min_cap) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Offset is 1; behaviour for n=0 not used. */
    /* Grow cache until we have n primes of the desired form. */
    while (cache_size < n) {
        /* Start checking from the next integer after last_checked. */
        uint64_t candidate = (uint64_t)last_checked + 1;
        if (is_prime_uint64(candidate) && has_form_prime(candidate)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size] = (seq_t)candidate;
            cache_size++;
        }
        last_checked = (seq_t)candidate;
    }
    return cache[n - 1];
}
