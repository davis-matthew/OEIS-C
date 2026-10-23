/*
 * A045197  Numbers whose base-5 representation contains exactly two 0's and
 *          two 3's.
 * https://oeis.org/A045197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑5 representation
 *            contains exactly two 0’s and two 3’s (found by brute‑force
 *            search)
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count = 0;   /* number of cached terms */
    static seq_t last_checked = 0;  /* last candidate examined */

    if (n == 0) return 0;  /* undefined for n < 1 */

    size_t target = (size_t)n;
    while (count < target) {
        /* Test the next integer. */
        last_checked = last_checked + 1;

        seq_t x = last_checked;
        unsigned zeros  = 0;
        unsigned threes = 0;
        while (x > 0) {
            unsigned digit = (unsigned)(x % (seq_t)5);
            if (digit == 0)    zeros++;
            else if (digit == 3) threes++;
            x = x / (seq_t)5;
        }

        /* Does it satisfy the digit condition? */
        if (zeros == 2 && threes == 2) {
            /* Grow cached array if necessary. */
            if (count + 1 > capacity) {
                size_t new_cap = capacity ? capacity << 1 : 16;
                if (new_cap < count + 1) new_cap = count + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                capacity = new_cap;
            }
            cache[count] = last_checked;
            ++count;
        }
    }
    return cache[target - 1];
}
