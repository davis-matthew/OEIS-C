/*
 * A130175  2n+1 appears (last digit of 2n+1) times .
 * https://oeis.org/A130175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 634, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 2n+1 appears (last digit of 2n+1) times. 
   a(1)=1, a(2)=3, a(3)=3, a(4)=3, a(5)=5, …
   The function builds the sequence lazily and keeps it cached
   for subsequent calls.         */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for generated terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* State for the next group to generate. */
    static idx_t next_odd = 1;     /* odd number to begin next group   */
    static idx_t curr_odd = 0;     /* odd number currently being used  */
    static idx_t times_left = 0;   /* repetitions remaining of curr_odd */

    /* undefined for n < 1 – return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Expand cache until it contains at least n terms. */
    while (len < (size_t)n) {
        /* Start a new group if we have no repetitions left. */
        if (times_left == 0) {
            curr_odd = next_odd;
            times_left = curr_odd % 10;   /* last digit repeats       */
            next_odd += 2;                /* next odd number          */
        }

        /* Ensure there's room for one more element. */
        if (len + 1 > cap) {
            size_t new_cap = cap == 0 ? 16 : cap << 1;
            if (new_cap < len + 1)
                new_cap = len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* out of memory               */
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = (seq_t)curr_odd;
        times_left--;
    }

    return cache[(size_t)n - 1];
}
