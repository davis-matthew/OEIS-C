/*
 * A023702  Numbers with exactly 4 2's in their ternary expansion of n.
 * https://oeis.org/A023702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023702: a(n) = the n-th integer whose ternary representation
   contains exactly four digits '2' */

#include <stdlib.h>
#include <stdint.h>

/* Count the number of digits equal to 2 in the base‑3 expansion of x. */
static int count_twos(seq_t x)
{
    int cnt = 0;
    while (x)
    {
        if ((x % 3) == 2)
            ++cnt;
        x /= 3;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* table of found terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static uint64_t next = 1;        /* candidate to test next */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        /* Search for the next integer with exactly four '2's in base 3. */
        while (1)
        {
            if (count_twos((seq_t)next) == 4)
            {
                /* Grow the storage by 64 entries when needed. */
                if (cache_len == 0 || cache_len % 64 == 0)
                {
                    size_t new_cap = cache_len + 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;   /* allocation failure – return 0 sentinel */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)next;
                ++next;
                break; /* found another valid term */
            }
            ++next;
        }
    }

    /* Return the n-th term (offset 1). */
    return cache[n - 1];
}
