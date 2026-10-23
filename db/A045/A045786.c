/*
 * A045786  Squares with initial digit '3'.
 * https://oeis.org/A045786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A045857(n)^2
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      = the square of the n‑th integer whose square begins with the digit 3 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of already found squares with leading digit 3          */
/* cache[i] holds a(i+1)                                               */
static seq_t *cache      = NULL;   /* array of cached values          */
static size_t capacity   = 0;      /* allocated size of cache         */
static size_t size       = 0;      /* number of terms stored          */
static uint64_t next     = 1;      /* next integer to test for leading 3 */

/* Helper: enlarge the cache to at least new_cap entries.  Copy the
   existing elements to the new block before freeing the old one.  */
static void grow_cache(void) {
    size_t new_cap = (capacity == 0) ? 8 : capacity << 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory: aborting – in the real test harness this will
           terminate the process, which is acceptable for a rare failure. */
        abort();
    }
    for (size_t i = 0; i < size; ++i)
        tmp[i] = cache[i];
    free(cache);
    cache    = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)         /* the sequence is defined for n ≥ 1, but return 0 for safety */
        return (seq_t)0;

    /* Extend cache until it contains at least n entries.          */
    while (size < n) {
        uint64_t m   = next++;          /* candidate integer to test   */
        seq_t sq = (seq_t)m * (seq_t)m;/* its square                   */

        /* Extract the most significant decimal digit of sq.         */
        seq_t temp = sq;
        while (temp >= (seq_t)10)
            temp /= (seq_t)10;
        int leading = (int)temp;        /* 0 … 9                       */

        if (leading == 3) {
            /* Store the square if it satisfies the property.       */
            if (size == capacity)
                grow_cache();
            cache[size++] = sq;
        }
    }

    return cache[n - 1];
}
