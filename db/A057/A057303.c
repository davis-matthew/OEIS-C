/*
 * A057303  Numbers n such that the number of distinct digits in n is a digit
 *          of n.
 * https://oeis.org/A057303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Find the n‑th integer such that the number of distinct
   decimal digits of the integer is a digit that occurs in the integer. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x satisfies the condition. */
static bool satisfies_seq_t(seq_t x)
{
    bool seen[10] = {0};
    seq_t t = x;
    int cnt = 0;

    while (t > 0) {
        int d = (int)(t % 10);   /* current least significant digit */
        if (!seen[d]) {
            seen[d] = true;
            ++cnt;
            if (cnt > 9) {
                /* More than 9 distinct digits -> impossible to have a
                   digit equal to the count (max digit is 9).  We may
                   break early, but we continue to consume the remaining
                   digits to keep the loop simple. */
            }
        }
        t /= 10;
    }

    if (cnt > 9) return false;   /* distinct digits 10 impossible */
    return seen[cnt];
}

/* Static cache of already computed terms.  The cache grows as needed. */
static seq_t *cache = NULL;          /* array of terms */
static idx_t cache_capacity = 0;     /* current allocated size */
static idx_t cache_size = 0;         /* number of terms stored */
static seq_t next_candidate = 1;     /* next integer to test */

/* Return the n-th term (1‑based) of sequence A057303. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Generate terms until we have enough */
    while (cache_size < n) {
        /* Ensure capacity for a new term */
        if (cache_size == cache_capacity) {
            idx_t new_cap = cache_capacity == 0 ? 64 : cache_capacity * 2;
            seq_t *p = mem_resize(seq_t, cache, new_cap);
            if (!p) abort();          /* out of memory */
            cache = p;
            cache_capacity = new_cap;
        }

        seq_t candidate = next_candidate;
        if (satisfies_seq_t(candidate)) {
            cache[cache_size++] = candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
