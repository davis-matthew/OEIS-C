/*
 * A037602  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0.
 * https://oeis.org/A037602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037602: Base‑9 digits are the periodic sequence 1,3,0.
   a(1) = 1
   For n > 1, a(n) = 9 * a(n-1) + d(n-1), where d(k) = {1,3,0}[k mod 3] */
#include <stdlib.h>

static seq_t *cache   = NULL;     /* cache[0] unused, cache[1] = a(1) */
static idx_t  cached_up_to = 0;   /* largest n for which cache[n] is defined */

static const unsigned char digit_cycle[3] = { 1, 3, 0 };

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence starts at offset 1 */

    /* extend cache if necessary */
    if (n > cached_up_to) {
        size_t new_size = (size_t)(n + 1) * sizeof(seq_t);

        /* Allocate or grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_size);
        if (tmp == NULL) {                   /* allocation failed */
            /* In a competition environment, aborting is safer than returning garbage */
            abort();
        }
        cache = tmp;

        /* Ensure base case for the first time */
        if (cached_up_to == 0) {
            cache[0] = 0;
        }

        /* Compute missing terms */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t digit = (seq_t)digit_cycle[(i - 1) % 3];
            cache[i] = cache[i - 1] * (seq_t)9 + digit;
        }

        cached_up_to = n;
    }

    return cache[n];
}
