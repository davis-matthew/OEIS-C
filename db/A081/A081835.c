/*
 * A081835  a(1)=1, a(n) = a(n-1) + 5 if n is already in the sequence, a(n) =
 *          a(n-1) + 4 otherwise.
 * https://oeis.org/A081835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + 5 if n is already present in the
 *            sequence a(n) = a(n-1) + 4 otherwise. (Offset 1)
 * Range:     u8 n <= 61, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of terms and a bitset of seen values.
       The cache uses 0-based indexing: cache[i] = a(i+1). */
    static seq_t *cache = NULL;           /* Terms of the sequence */
    static idx_t cache_capacity = 0;      /* Allocated size of cache */
    static idx_t cache_size = 0;          /* Number of terms already stored */

    static unsigned char *seen = NULL;    /* seen[v] == 1 -> value v has appeared */
    static idx_t seen_capacity = 0;      /* Allocated size of seen array */

    /* Ensure capacity for the 'seen' bitset. */
    if (seen_capacity < n + 1) {
        idx_t new_cap = seen_capacity ? seen_capacity << 1 : 128;
        while (new_cap <= n) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
        if (!tmp) return (seq_t)-1;              /* Caught allocation failure */
        memset(tmp + seen_capacity, 0, new_cap - seen_capacity);
        seen = tmp;
        seen_capacity = new_cap;
    }

    /* Initialize the sequence for n = 1. */
    if (cache_size == 0) {
        /* Allocate at least one term. */
        cache_capacity = 128;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)-1;            /* allocation failure */
        cache[0] = (seq_t)1;                    /* a(1) = 1 */
        cache_size = 1;
        /* Mark value 1 as seen. */
        if (seen_capacity <= 1) {
            idx_t new_cap = 2;
            unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
            if (!tmp) return (seq_t)-1;
            memset(tmp + seen_capacity, 0, new_cap - seen_capacity);
            seen = tmp;
            seen_capacity = new_cap;
        }
        seen[1] = 1;
    }

    /* Extend the precomputation up to the requested index n. */
    while (cache_size < n) {
        idx_t idx = cache_size + 1;                 /* Current index (1-based) */
        /* Check if idx is already present in the sequence. */
        bool present = (idx < seen_capacity) ? (seen[idx] != 0) : false;

        seq_t prev = cache[cache_size - 1];
        seq_t term = prev + (present ? (seq_t)5 : (seq_t)4);

        /* Store new term. */
        if (cache_size >= cache_capacity) {
            idx_t new_cap = cache_capacity ? cache_capacity << 1 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)-1;
            cache = tmp;
            cache_capacity = new_cap;
        }
        cache[cache_size] = term;
        cache_size++;

        /* Mark the newly produced value as seen. */
        if (term >= seen_capacity) {
            idx_t new_cap = seen_capacity ? seen_capacity << 1 : 128;
            while (new_cap <= term) new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
            if (!tmp) return (seq_t)-1;
            memset(tmp + seen_capacity, 0, new_cap - seen_capacity);
            seen = tmp;
            seen_capacity = new_cap;
        }
        seen[(idx_t)term] = 1;
    }

    return cache[n - 1];
}
