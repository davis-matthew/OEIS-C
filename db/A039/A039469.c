/*
 * A039469  Numbers whose base-11 representation has the same nonzero number
 *          of 3's and 8's.
 * https://oeis.org/A039469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑11 representation has the same nonzero
 * number of 3's and 8's – computed by brute‑force search. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global cache of found terms.  The sequence is generated on demand.
 * cache  : pointer to an array of seq_t
 * cap    : capacity of the array in elements
 * len    : number of elements currently stored (i.e. next index to fill)
 * tested : smallest integer not yet tested for the property */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;
static idx_t tested = 1;

seq_t a(idx_t n)
{
    /* generate terms up to and including the nth one if required */
    while (len < n) {
        /* count digits 3 and 8 in the base‑11 representation of tested */
        idx_t cnt3 = 0, cnt8 = 0;
        seq_t tmp = (seq_t)tested;
        if (tmp == 0) {
            tested++;
            continue;
        }
        while (tmp) {
            seq_t rem = tmp % 11;
            if (rem == 3)
                cnt3++;
            else if (rem == 8)
                cnt8++;
            tmp /= 11;
        }
        if (cnt3 > 0 && cnt8 > 0 && cnt3 == cnt8) {
            /* ensure capacity and append the new term */
            if (len == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                /* cast explicitly for C++ compatibility */
                seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newptr) {
                    /* allocation failure – return a zero value (safe for integers) */
                    return (seq_t)0;
                }
                cache = newptr;
                cap = newcap;
            }
            cache[len] = (seq_t)tested;
            len++;
        }
        tested++;
    }
    return cache[n - 1];
}
