/*
 * A044209  Numbers n such that string 2,6 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search for numbers whose base‑8 representation contains the
 *            substring "26" but the preceding number does not. The sequence
 *            a(n) is obtained by finding all such numbers in increasing order
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_sub26_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_sub26_oct.h"

/* static cache for already computed terms */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t count = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1; n=0 not used */

    /* extend the cache until we have at least n terms */
    while (count < (size_t)n) {
        /* start searching from the next integer after the last cached value */
        seq_t start = (count == 0) ? 1 : cache[count - 1] + 1;

        for (seq_t x = start; ; ++x) {
            if (has_sub26_oct(x) && !has_sub26_oct(x - 1)) {
                /* ensure we have room in the cache */
                if (count >= cap) {
                    size_t new_cap = (cap == 0) ? 8 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* allocation failure – abort */
                        abort();
                    }
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[count] = x;
                ++count;
                break; /* move on to the next term */
            }
        }
    }
    return cache[n - 1];
}
