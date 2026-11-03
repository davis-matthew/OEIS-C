/*
 * A063538  Numbers n that are not sqrt(n-1)-smooth: largest prime factor of n
 *          (=A006530(n)) >= sqrt(n).
 * https://oeis.org/A063538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 7809 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063538: a(n) = the nth integer ≥ 2 whose largest prime factor is ≥ √m.
   The property “lpf(m)² ≥ m” is equivalent to the original definition. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the largest prime factor of x.  Works for any unsigned
   integer type seq_t.  Returns 1 for x = 0 or 1, but those values
   are never used. */
static seq_t lpf_seq_t(seq_t x)
{
    seq_t maxp = 1;
    while ((x & 1) == 0) {          /* factor out 2 */
        maxp = 2;
        x >>= 1;
    }
    for (seq_t p = 3; p <= x / p; p += 2) {
        while (x % p == 0) {
            maxp = p;
            x /= p;
        }
    }
    if (x > 1)  /* remaining prime */
        maxp = x;
    return maxp;
}

/* Dynamic array storing the sequence values already generated. */
static seq_t *seq_cache = NULL;   /* array of terms */
static size_t  seq_len   = 0;     /* number of stored terms */
static size_t  seq_cap   = 0;     /* capacity of seq_cache */
static seq_t  next_test  = 2;     /* next integer to test */

/* Append a value to the dynamic cache. */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        if (!tmp) { /* allocation failed; abort */
            abort();
        }
        seq_cache = tmp;
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* According to OEIS, a(0) is undefined; we return 0. */
        return (seq_t)0;
    }

    /* Grow the cache until we have at least n terms. */
    while (seq_len < (size_t)n) {
        seq_t m = next_test++;
        seq_t maxp = lpf_seq_t(m);
        /* m qualifies iff maxp² ≥ m. */
        if (maxp * maxp >= m)
            seq_append(m);
    }
    return seq_cache[n - 1];
}
