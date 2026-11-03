/*
 * A070807  Composite numbers n such that Cototient(totient(n))=A070556(n) is
 *          power of 2.
 * https://oeis.org/A070807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth composite integer n such that the cototient of
 *            φ(n) – that is, φ(n) – φ(φ(n)) – is a power of two. The first
 *            terms are
 *            4,6,8,9,10,12,14,15,16,18,20,21,24,26,28,30,32,34,35,36,39,40,42,45,
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_pow2, mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_pow2.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;   /* Cache of found sequence terms */
static size_t cache_len = 0;  /* Number of terms stored */
static size_t cache_cap = 0;  /* Current capacity of cache */
static idx_t next_n = 4;      /* Next integer to test (first composite) */

/* Ensure the cache has room for at least one more element */
static void init_cache(void)
{
    if (!cache) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    }
}

/* Append a new term to the cache, expanding it if necessary */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap <<= 1;                       /* double the capacity */
        cache = mem_resize(seq_t, cache, cache_cap);
    }
    cache[cache_len++] = val;
}

/* Test whether a seq_t value is a power of two (including 1). */

/* Requested function.  Builds the sequence on demand. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;

    init_cache();

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* phi(n) */
        seq_t phi_n = totient(next_n);

        /* Composite test: φ(n) == n-1 only for prime n (and 2). */
        if (phi_n != (seq_t)(next_n - 1)) {
            /* cototient of φ(n)  = φ(n) – φ(φ(n)) */
            seq_t phi_phi = totient((idx_t)phi_n);
            seq_t cot = phi_n - phi_phi;

            if (is_pow2(cot))
                cache_append(next_n);
        }
        ++next_n;
    }

    return cache[n - 1];
}
