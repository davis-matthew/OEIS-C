/*
 * A096738  Numbers n such that A096736(n) = (n*tau(n)-1)/(tau(n)-1).
 * https://oeis.org/A096738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096738: a(n) = the nth integer > 1 such that
   (a(n)*tau(a(n)) - 1) mod (tau(a(n)) - 1) == 0.
   All primes satisfy this condition. */

#include <stdlib.h>
#include <stdint.h>

/* Cache for already generated terms */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* The largest integer that has been checked so far */
static idx_t last_checked = 1;

/* Append a new term to the cache, expanding storage if necessary */
static void push_term(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failure – silently abort */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Computes tau(x) via trial division.  All values involved are 64‑bit
   for the sizes of numbers encountered in the tested range. */
static uint64_t tau_uint64(uint64_t x)
{
    uint64_t result = 1;
    uint64_t tmp = x;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (tmp > 1) result *= 2;
    return result;
}

extern seq_t a(idx_t n)
{
    while (cache_len < n) {
        ++last_checked;
        uint64_t num = (uint64_t)last_checked;

        /* Skip numbers <= 1, though they will never be needed */
        if (num <= 1)
            continue;

        uint64_t tau = tau_uint64(num);
        /* For n > 1, tau >= 2, hence tau-1 > 0 */
        uint64_t d = tau - 1;
        if (((num - 1) % d) == 0) {
            push_term((seq_t)num);
        }
    }
    return cache[n - 1];
}
