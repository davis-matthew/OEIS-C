/*
 * A096016  A modified Fibonacci sequence controlled by a toggle switch. The
 *          toggle switch (initial state of 2) flips between 2 and 3 after
 *          each reduction.
 * https://oeis.org/A096016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 47, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096016 – Modified Fibonacci with a toggle switch
   Recurrence:
   a(0) = 0,  a(1) = 1
   For n ≥ 2
       s = a(n‑1) + a(n‑2)
       if s > 2 and toggle == 2 and s % 2 == 0:
           s = s / 2
           toggle = 3
       else if s > 3 and toggle == 3 and s % 3 == 0:
           s = s / 3
           toggle = 2
       a(n) = s
   The toggle begins at 2 and flips to the other value only when a
   reduction occurs.  The function memoises results so that calls
   can be made in any order.  The implementation works for any
   integer type `seq_t` that supports +, -, *,/, % and comparison. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state that persists over calls */
    static seq_t *cache      = NULL;   /* cached a(k) values   */
    static idx_t cache_size  = 0;      /* current allocated size */
    static idx_t max_comp    = 0;      /* largest n computed   */
    static int  toggle       = 2;      /* current toggle state */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Ensure room for a[0] and a[1] at least */
        cache_size = (n + 1 > 2) ? (n + 1) : 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_comp = 1;                  /* we have a[0] and a[1] ready */
        toggle   = 2;
    }

    /* Grow the cache if the requested index is larger than current size */
    if ((idx_t)n + 1 > cache_size) {
        idx_t new_size = (idx_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute missing entries up to n */
    if ((idx_t)n > max_comp) {
        for (idx_t i = max_comp + 1; i <= (idx_t)n; ++i) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            if (sum > (seq_t)2 && toggle == 2 && sum % (seq_t)2 == 0) {
                sum /= (seq_t)2;
                toggle = 3;
            } else if (sum > (seq_t)3 && toggle == 3 && sum % (seq_t)3 == 0) {
                sum /= (seq_t)3;
                toggle = 2;
            }
            cache[i] = sum;
        }
        max_comp = (idx_t)n;
    }

    return cache[(idx_t)n];
}
