/*
 * A058414  Squares composed of digits {0,1,4}, not ending with zero.
 * https://oeis.org/A058414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A058413(n)^2. * A058413(n) is the nth positive integer
 *            whose square * consists solely of decimal digits 0,1,4 and does
 *            not end with 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* stored squares */
static size_t cache_size = 0;        /* current number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t next_root = 1;          /* next candidate root to test */

/* grow cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t new_capacity = cache_cap ? cache_cap * 2 : 64;
    while (new_capacity < new_cap) new_capacity <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp) exit(1);   /* abort on allocation failure */
    cache = tmp;
    cache_cap = new_capacity;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* outside defined offset */

    /* extend cache until we have at least n terms */
    while (cache_size < n) {
        idx_t ld = next_root % 10;
        if (ld == 1 || ld == 2 || ld == 8 || ld == 9) {
            seq_t sq = (seq_t)next_root * (seq_t)next_root;

            /* check that the square uses only digits 0,1,4 */
            bool ok = true;
            seq_t t = sq;
            while (t > 0) {
                seq_t d = t % 10;
                if (d != 0 && d != 1 && d != 4) {
                    ok = false;
                    break;
                }
                t /= 10;
            }

            if (ok) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = sq;
            }
        }
        ++next_root;        /* move to the next candidate root */
    }

    return cache[n - 1];
}
