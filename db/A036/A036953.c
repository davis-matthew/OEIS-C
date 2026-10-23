/*
 * A036953  Primes having only {0, 1, 2} as digits.
 * https://oeis.org/A036953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (n‑th) prime whose decimal digits belong to {0,1,2}
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 2341, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates numbers of increasing length whose
 * digits are 0, 1 or 2.  For each length it recursively builds the
 * numbers in lexicographic order, guaranteeing numeric order within
 * that length.  All generated numbers ending with 1 or 2 (except the
 * single‑digit 2) are tested for primality with the provided
 * isPrimeGeneric() routine.  The primes found are appended to a
 * static cache so that subsequent calls can retrieve earlier terms
 * without recomputation.  The generation continues until the cache
 * holds at least the requested index. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

/* Static cache for terms.  cache, cache_size and cache_cap are
 * maintained by the generator.  current_len tracks the length of
 * numbers currently being generated. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t current_len = 1;          /* next length to generate */

/* Recursive helper to generate all numbers of given length.
 * It updates the global cache via append_candidate. */
static void gen_rec(idx_t pos, seq_t val, idx_t len)
{
    if (pos == len) {
        /* All digits chosen; test primality. */
        if (isPrimeGeneric(val))
            append_candidate(&cache, &cache_size, &cache_cap, val);
        return;
    }

    /* Determine allowed digits for this position. */
    int start = 0;
    int end   = 3;          /* exclusive upper bound */

    if (pos == 0)          /* most significant digit cannot be 0 */
        start = 1;
    if (pos == len - 1)     /* least significant must be 1 or 2 */
        start = 1;
    /* For length 1 we want only the digit 2 */
    if (len == 1)
        start = 2;

    for (int d = start; d < end; ++d)
        gen_rec(pos + 1, val * 10 + d, len);
}

/* Generate all numbers of a specific length and append primes. */
static void generate_len(idx_t len)
{
    if (len == 1) {
        /* Special case: only the number 2 is allowed. */
        seq_t val = 2;
        if (isPrimeGeneric(val))
            append_candidate(&cache, &cache_size, &cache_cap, val);
    } else {
        /* Recursively build numbers of length len. */
        gen_rec(0, 0, len);
    }
}

/* Ensure that the cache contains at least all terms up to target index. */
static void ensure_terms(idx_t target)
{
    if (target <= cache_size)
        return;                 /* already available */

    while (cache_size < target) {
        generate_len(current_len);
        ++current_len;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, return 0 */
    ensure_terms(n);
    return cache[n - 1];
}
