/*
 * A031455  Numbers whose base-3 representation has 3 more 0's than 2's.
 * https://oeis.org/A031455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑3 representation
 *            contains exactly three more 0’s than 2’s
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* check if x satisfies the property (zeros = twos + 3) */
static bool has_property(seq_t x)
{
    unsigned long long zeros = 0, twos = 0;
    seq_t tmp = x;
    /* a base‑3 digit cannot be leading 0 because we start from 1
       and we ignore the most significant 0 when x==0 (not used).  */
    while (tmp != 0) {
        int d = (int)(tmp % 3);           /* tmp % 3 fits in an int */
        if (d == 0)   zeros++;
        else if (d == 2) twos++;
        tmp /= 3;
    }
    return zeros == twos + 3;
}

/* cache of already found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* allocated capacity */
static seq_t next_candidate = 1; /* next integer to test */

/* grow the cache when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* produce the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, return 0 for safety */

    /* extend cache until we have at least n terms */
    while (cache_size < n) {
        /* search for the next qualifying integer */
        while (1) {
            seq_t x = next_candidate++;
            if (has_property(x)) {
                if (cache_size == cache_cap) grow_cache();
                cache[cache_size++] = x;
                break;   /* found next term, go generate the next one */
            }
        }
    }
    return cache[n - 1];
}
