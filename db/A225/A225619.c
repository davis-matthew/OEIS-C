/*
 * A225619  Composite numbers that remain composite if any digit is deleted
 *          (zero and one are not considered prime).
 * https://oeis.org/A225619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th composite number such that deleting any single digit
 *            leaves a number that is composite (0 and 1 are not considered
 *            prime)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, isPrimeGeneric, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;        /* gathered terms */
static size_t cache_len = 0;       /* number of stored terms */
static size_t cache_cap = 0;       /* current allocation size */
static idx_t next_val = 0;         /* next integer to test (as idx_t) */

/* Helper: test if a number is composite (i.e., >1 and not prime). */
static bool is_composite(seq_t v)
{
    if (v <= 1)
        return false;
    return !isPrimeGeneric(v);
}

seq_t a(idx_t n)
{
    /* Build the cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t x = (seq_t)next_val++;   /* candidate number */

        if (!is_composite(x))
            continue;                  /* skip primes and ≤1 */

        bool ok = true;
        int len = digit_len(x);

        /* Check deletions of all digit positions. */
        for (int pos = 0; pos < len && ok; ++pos) {
            seq_t pow10pos  = pow10((idx_t)pos);
            seq_t pow10next = pow10((idx_t)(pos + 1));

            seq_t low  = x % pow10pos;
            seq_t high = x / pow10next;
            seq_t deleted = high * pow10pos + low;

            if (!is_composite(deleted))
                ok = false;
        }

        if (ok) {
            /* Resize cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
    }
    return cache[n - 1];
}
