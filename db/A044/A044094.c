/*
 * A044094  Numbers k such that string 3,2 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2780, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Numbers k such that the base‑5 representation of k contains the
 * substring "32" but k-1 does not.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true iff num contains the consecutive digits 3,2 in its
   base‑5 representation.  The test walks the number from least
   significant digit upward.  A digit 3 followed by a digit 2
   (more significant) yields the pattern "32". */
static bool contains32(seq_t num)
{
    if (num < (seq_t)5) return false;    /* need at least two digits */

    seq_t prev = num % (seq_t)5;        /* least significant digit */
    num /= (seq_t)5;
    while (num > (seq_t)0) {
        seq_t cur = num % (seq_t)5;    /* next more significant digit */
        if (cur == (seq_t)3 && prev == (seq_t)2)
            return true;
        prev = cur;
        num /= (seq_t)5;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache   = NULL;   /* array of found numbers   */
    static size_t cache_cap = 0;    /* allocated capacity      */
    static size_t cache_size = 0;   /* how many elements filled*/
    static seq_t next_k = 1;        /* next integer to test    */
    static bool prev_has = false;   /* does k-1 contain "32"?  */

    if (n <= 0)
        return 0;                  /* out of sequence range   */

    /* Ensure we have at least n terms in the cache */
    while (cache_size < n) {

        /* Check the current integer */
        bool cur_has = contains32(next_k);
        if (cur_has && !prev_has) {
            /* Grow capacity if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_k;
        }

        /* Prepare for next iteration */
        prev_has = cur_has;
        ++next_k;
    }

    return cache[n - 1];
}
