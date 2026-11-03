/*
 * A071638  Numbers n such that phi(n) = b(n,1)^b(n,0) where b(n,1) is the
 *          number of 1's in binary representation of n and b(n,0) the number
 *          of 0's.
 * https://oeis.org/A071638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer m such that φ(m) = b(m,1)^b(m,0) where
 *            b(m,1) is the number of 1‑bits in the binary representation of m
 *            and b(m,0) the number of 0‑bits. Indices start at 1
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Utility: grow a dynamic array of seq_t values.  Works in C and
   C++ without requiring constructors because it only manipulates
   trivially constructible types (seq_t may be integral or a big
   integer class that supports simple copying). */
static void cache_push(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *cache, newcap);
        if (!tmp) abort();          /* out of memory */
        *cache = tmp;
        *cap   = newcap;
    }
    (*cache)[*len] = val;
    ++(*len);
}

/* Helper: compute base^exp, stopping as soon as the result would
   exceed the supplied limit.  Works for arbitrary-precision
   seq_t as well.  If the true value exceeds limit the function
   returns limit+1. */
static seq_t pow_limit(seq_t base, uint64_t exp, seq_t limit)
{
    seq_t result = 1;
    bool overflow = false;
    while (exp) {
        if (exp & 1ULL) {
            seq_t tmp = result * base;
            if (tmp > limit) { overflow = true; break; }
            result = tmp;
        }
        exp >>= 1ULL;
        if (exp) {
            seq_t tmp = base * base;
            /* If next square would already exceed limit, set base to a value
               that guarantees overflow in the next multiplication. */
            if (tmp > limit) { base = limit + 1; continue; }
            base = tmp;
        }
    }
    return overflow ? (limit + 1) : result;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* The sequence function.  It lazily generates terms and caches them.
   Because the caller may request terms in any order, caching ensures
   each term is evaluated only once. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t len   = 0;      /* number of cached terms */
    static size_t cap   = 0;      /* capacity of cache array */
    static idx_t next_candidate = 1;  /* next integer to test */

    /* The sequence is defined for n >= 1 only. */
    if (n == 0) return 0;

    /* Build terms until we have at least n of them. */
    while (len < (size_t)n) {
        idx_t x = next_candidate++;
        /* Count bits in binary representation of x. */
        unsigned bits  = 64 - __builtin_clzll((unsigned long long)x);
        unsigned ones  = __builtin_popcountll((unsigned long long)x);
        unsigned zeros = bits - ones;

        seq_t phi_val = totient(x);                       /* φ(x) from utils */
        seq_t pow_val = pow_limit((seq_t)ones, zeros, phi_val);

        if (pow_val == phi_val) {
            cache_push((seq_t)x, &cache, &len, &cap);
        }
    }
    return cache[n - 1];
}
