/*
 * A118457  Table of partitions of n into distinct parts, in Mathematica
 *          ordering.
 * https://oeis.org/A118457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9851, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A118457: list of all parts of every partition of k
   into distinct parts, where partitions are output in reverse‑lexicographic
   order and the list of parts for each k is appended to a single array.
   The function a(n) returns the n‑th element of that concatenated array
   (1‑based indexing).  The implementation uses a simple recursive
   partition generator and memoises the accumulated list so that
   subsequent calls re‑use previously computed rows. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;    /* cache holds a(1)..a(cache_len)          */
static size_t cache_len = 0;   /* number of elements currently stored    */
static size_t cache_cap = 0;   /* allocated capacity for the cache        */
static idx_t cur_k = 0;        /* largest k for which rows have been added*/

/* Ensure the cache can hold at least 'min_cap' elements.  */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    /* realloc must be cast because seq_t may be a user‑defined class in C++ */
    seq_t *p = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();          /* memory allocation failure */
    cache = p;
    cache_cap = new_cap;
}

/* Append a single part value to the cache. */
static void append_part(idx_t v)
{
    if (cache_len == cache_cap) grow_cache(cache_len + 1);
    cache[cache_len++] = (seq_t)v;
}

/* Recursive generation of partitions of 'rem' using parts ≤ 'max_part'.
   The current partial partition is held in 'stack[0..depth]'. */
static void gen(idx_t rem, idx_t max_part, idx_t *stack, idx_t depth)
{
    for (idx_t p = max_part; p > 0; --p) {
        stack[depth] = p;
        if (rem == p) {              /* complete partition */
            for (idx_t i = 0; i <= depth; ++i)
                append_part(stack[i]);
        } else if (rem > p) {       /* more parts to add */
            gen(rem - p, p - 1, stack, depth + 1);
        }
    }
}

/* Generate all partitions of the integer 'k' into distinct parts
   in reverse‑lexicographic order and append their parts to the cache. */
static void generate_row(idx_t k)
{
    /* allocate a stack large enough to hold the largest possible
       partition (length ≤ k).  Use a cast because idx_t may be a
       user‑defined integer type in C++. */
    idx_t *stack = (idx_t*)malloc((k + 1) * sizeof(idx_t));
    if (!stack) abort();
    gen(k, k, stack, 0);
    free(stack);
}

/* Public function required by the build environment.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* index 0 is unused */
    while (cache_len < n) {                  /* compute rows as needed */
        ++cur_k;
        generate_row(cur_k);
    }
    return cache[n - 1];                     /* 1‑based indexing */
}
