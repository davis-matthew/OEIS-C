/*
 * A161979  a(1)=1. a(n) = the smallest integer > a(n-1) containing {the
 *          string made by reversing the order of the binary digits of n, and
 *          removing leading 0's} as a substring when a(n) is written in
 *          binary.
 * https://oeis.org/A161979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is defined by recurrence. a(1) = 1. For n > 1, a(n) is the
 *            smallest integer > a(n-1) whose binary representation contains
 *            the reverse (without leading zeros) of the binary representation
 *            of n as a substring
 * Range:     u8 n <= 56, u16 n <= 736, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Forward declarations of helper functions. */
static seq_t rev_bin(idx_t n);
static void ensure_cache(idx_t need);

/* Cache for previously computed terms. */
static seq_t *seq_cache = NULL;
static idx_t  seq_cache_cap = 0;

/* Ensure that the cache contains storage up to index 'need'. */
static void ensure_cache(idx_t need)
{
    if (seq_cache_cap > need)
        return;

    idx_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 1;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t *tmp = NULL;
#ifdef __cplusplus
    tmp = new seq_t[new_cap];
    for (idx_t i = 0; i < seq_cache_cap; ++i)
        tmp[i] = seq_cache[i];
    delete [] seq_cache;
    for (idx_t i = seq_cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
#else
    tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    for (idx_t i = seq_cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
#endif
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* Return the binary reversal of n (without leading zeros). */
static seq_t rev_bin(idx_t n)
{
    seq_t rev = 0;
    while (n)
    {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return rev;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1 */

    ensure_cache(n);

    if (seq_cache[n])
        return seq_cache[n];

    if (n == 1)
    {
        seq_cache[1] = 1;
        return 1;
    }

    /* Ensure the previous term is computed. */
    if (!seq_cache[n-1])
        a(n-1);

    const seq_t prev = seq_cache[n-1];
    const seq_t pattern = rev_bin(n);
    const unsigned patlen = bitlen_seq_t(pattern);

    /* Build a bitmask covering patlen lower bits. */
    seq_t mask;
    if (patlen == (unsigned)(sizeof(seq_t) * 8))
        mask = ~((seq_t)0);
    else
        mask = (((seq_t)1 << patlen) - 1);

    seq_t candidate = prev + 1;
    while (true)
    {
        const unsigned candlen = bitlen_seq_t(candidate);
        if (candlen >= patlen)
        {
            bool found = false;
            for (idx_t shift = 0; shift <= candlen - patlen; ++shift)
            {
                if (((candidate >> shift) & mask) == pattern)
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                seq_cache[n] = candidate;
                return candidate;
            }
        }
        ++candidate;
    }
}
