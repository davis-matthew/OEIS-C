/*
 * A057890  In base 2, either a palindrome or becomes a palindrome if trailing
 *          0's are omitted.
 * https://oeis.org/A057890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 1754, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A057890 – numbers whose binary representation becomes a palindrome
 *  after dropping all trailing zeroes.
 *
 *  The sequence includes 0, all odd palindromes, and also numbers that are
 *  an odd palindrome multiplied by a power of 2.
 *
 *  Implementation strategy:
 *  • Keep a cache of the terms computed so far.
 *  • When a larger index is requested, extend the cache by scanning
 *    successive integers, testing each one.
 *  • A number x is accepted if
 *        y = x with all trailing zeros removed
 *      and y’s binary digits read the same forwards and backwards.
 *  • The palindrome test uses the provided bit‑length routine
 *    `bitlen_seq_t` and simple bit inspections.
 *
 *  Complexity:  O(n) to generate the first n terms; subsequent
 *  queries are answered in O(1).   Suitable for n up to ~10⁴
 *  within the time limits.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Check whether an integer x (seq_t) is in the sequence.
 * 0 is always accepted.  For x>0 we remove trailing zeros
 * and test whether the remaining odds part is a binary palindrome.
 */
static bool in_sequence(seq_t x)
{
    if (x == (seq_t)0)
        return true;

    /* strip trailing zero bits */
    seq_t y = x;
    while ((y & (seq_t)1) == (seq_t)0)
        y >>= 1;

    /* y is odd (last bit 1).  Test palindrome. */
    unsigned len = bitlen_seq_t(y);
    unsigned i, j;
    for (i = 0, j = len - 1; i < j; ++i, --j) {
        if ( ((y >> i) & (seq_t)1) != ((y >> j) & (seq_t)1) )
            return false;
    }
    return true;
}

/* Cache of computed terms */
static seq_t *cache   = NULL;   /* cache[0] holds a(1) */
static size_t cached  = 0;      /* number of terms currently stored */
static size_t capacity = 0;     /* allocated capacity of cache array */

/* Last integer examined when extending the cache.  It holds the value
 * of the last number that was tested and included in the cache. */
static seq_t last_x = 0;

/* Ensure that the cache contains at least n terms (n>=1). */
static void ensure_cache(idx_t n)
{
    if (n <= cached)
        return;

    /* Initialize the cache if it is empty.  The very first term is 0. */
    if (cached == 0) {
        capacity = 16;               /* start with a small capacity */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;         /* a(1) */
        cached = 1;
        last_x  = (seq_t)0;
    }

    /* Extend the cache until we have at least n terms. */
    while (cached < n) {
        /* Generate the next eligible number.  We start from
           last_x + 1 and scan forward until we find a number in
           the sequence.  The linear scan is inexpensive for n~10⁴. */
        seq_t candidate = last_x + 1;
        while (!in_sequence(candidate))
            ++candidate;

        /* Append the new term. */
        if (cached >= capacity) {
            capacity = capacity * 2 + 16;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        }
        cache[cached] = candidate;
        ++cached;
        last_x = candidate;
    }
}

/* Main entry function required by the test harness. */
seq_t a(idx_t n)
{
    /* n is 1‑based.  Ensure the cache holds at least n terms. */
    ensure_cache(n);

    /* Retrieve the requested term.  cache[0] = a(1). */
    return cache[n - 1];
}
