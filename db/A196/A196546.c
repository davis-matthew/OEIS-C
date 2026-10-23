/*
 * A196546  Numbers n such that the sum of the distinct residues of x^n (mod
 *          n), x=0..n-1, is divisible by n.
 * https://oeis.org/A196546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A196546: Numbers n such that the sum of the distinct residues of
 * x^n (mod n), x=0..n-1, is divisible by n.
 * The function a(n) returns the n-th term (1‑based index). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static bool qualifies(idx_t n)
{
    /* Special case n = 1: sum of distinct residues is 0, divisible by 1. */
    if (n == 1)
        return true;

    /* Allocate an array to mark which residues modulo n have already
     * appeared.  n is small (the 10 000‑th term is ≈ 2·10^4), so this
     * allocation is safe. */
    size_t sz = (size_t)n;
    uint8_t *seen = (uint8_t *)calloc(sz, 1);
    if (!seen)
        return false;                    /* allocation failure → reject */

    seq_t modN = (seq_t)n;                /* for modulo arithmetic */
    seq_t sum_mod = 0;                    /* running sum modulo n */

    for (idx_t x = 0; x < n; ++x) {
        seq_t r = mod_pow((seq_t)x, n, modN);  /* x^n mod n */

        if (!seen[(size_t)r]) {          /* new distinct residue */
            seen[(size_t)r] = 1;
            sum_mod += r;
            sum_mod %= modN;             /* keep the sum small */
        }
    }

    free(seen);
    return sum_mod == 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next_candidate = 1;

    if (cache_len < n) {
        /* Grow cache if necessary. */
        if (cache_cap < n * 2) {
            idx_t new_cap = n * 2;
            seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)new_cap * sizeof(seq_t));
            if (!new_cache)
                return (seq_t)0;          /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Generate terms until we have the required number. */
        while (cache_len < n) {
            bool ok = qualifies(next_candidate);
            if (ok)
                cache[cache_len++] = (seq_t)next_candidate;
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
