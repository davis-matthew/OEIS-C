/*
 * A059644  Primes p such that x^58 = 2 has no solution mod p.
 * https://oeis.org/A059644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 950 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A059644 – primes p such that the congruence x^58 ≡ 2 (mod p)
   has no solution. The function a(idx_t n) returns the nth term,
   with a(1) = 3. */
    
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

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

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t *cache   = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t  next     = 3;          /* next odd integer to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined for n = 0 */

    while (cache_len < n) {
        if (is_prime(next)) {
            idx_t g   = gcd_idx(next - 1, (idx_t)58);
            uint64_t exp = (next - 1) / g;
            seq_t r  = powmod((seq_t)2, exp, (seq_t)next);
            if (r != (seq_t)1) {     /* no solution to x^58 ≡ 2 (mod next) */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = seq_realloc(cache, cache_len, cache_cap);
                    if (!cache)
                        return (seq_t)0; /* allocation failure */
                }
                cache[cache_len++] = (seq_t)next;
            }
        }
        next += 2;   /* skip even numbers */
    }
    return cache[n - 1];
}
