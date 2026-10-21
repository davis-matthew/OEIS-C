/*
 * A083494  Multiples of 6 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A083494: Multiples of 6 in which there is no common digit in successive terms.
   a(0) = 6
   a(n) = the smallest multiple of 6 greater than a(n-1) that shares no decimal digit with a(n-1).  */
#include <stdlib.h>
#include <stdbool.h>

/* Determine whether two numbers share a decimal digit.
   Works for any seq_t that supports modulus 10 and division by 10. */
static bool common_digits(seq_t a, seq_t b)
{
    unsigned char mask[10] = {0};
    seq_t t = a;

    if (t == (seq_t)0)      /* number 0 has a single digit 0 */
        mask[0] = 1;
    while (t > (seq_t)0) {
        int d = (int)(t % (seq_t)10);
        mask[d] = 1;
        t /= (seq_t)10;
    }

    t = b;
    if (t == (seq_t)0)
        return mask[0];
    while (t > (seq_t)0) {
        int d = (int)(t % (seq_t)10);
        if (mask[d])
            return true;
        t /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    if (len == 0) {               /* initialise with first term 6 */
        cap   = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)6;
        len = 1;
    }

    while (len <= n) {            /* extend cache up to n */
        seq_t cur = cache[len-1];
        seq_t cand = cur + (seq_t)6;       /* next multiple of 6 */

        while (common_digits(cur, cand))
            cand += (seq_t)6;          /* find next suitable candidate */

        if (len == cap) {            /* grow array if needed */
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len++] = cand;          /* store new term */
    }

    return cache[n];
}
