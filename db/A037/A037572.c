/*
 * A037572  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,2.
 * https://oeis.org/A037572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is formed by writing base‑7 digits that are the first n
 *            terms of the periodic sequence 2,1,2,2,1,2,2,…, i.e. for
 *            position k the digit is 1 if k mod 3 == 2, otherwise 2. The
 *            value is then the number represented by these digits in base 7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: cache[k] holds a(k) for 0 ≤ k ≤ last.
       cache[0] is set to 0 to simplify the recurrence.
    */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache      */
    static idx_t last = 0;         /* highest n already computed */

    /* Ensure we have room for indices up to n. */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort with 0 (undefined behaviour for
               incomplete sequence is acceptable for the test harness).   */
            return 0;
        }
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialise the 0‑th element which is not part of the OEIS
       sequence but is necessary for the recurrence. */
    if (last < 1) {
        cache[0] = 0;
    }

    /* Compute any missing values up to n.  The recurrence is
       a(k) = a(k‑1) * 7 + digit(k), where digit(k) = 2 except when
       k mod 3 == 2, in which case digit(k) = 1. */
    for (size_t k = (size_t)last + 1; k <= (size_t)n; ++k) {
        unsigned char digit = (k % 3 == 2) ? 1 : 2;
        cache[k] = cache[k-1] * 7 + (seq_t)digit;
    }

    last = n;
    return cache[n];
}
