/*
 * A043807  Numbers k such that number of runs in the base 3 representation of
 *          k is congruent to 1 mod 9.
 * https://oeis.org/A043807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043807:
   a(n) = the nth integer k such that the number of runs in the base‑3
   representation of k is congruent to 1 modulo 9.  The sequence is
   defined by a search: we list all integers k, count the runs of
   consecutive identical digits in base 3, and accept k when
   runs % 9 == 1.  We store the accepted values in a static cache so
   that a calls may arrive in any order. */

#include <stdlib.h>
#include <assert.h>

#define CACHE_MAX 200000   /* safe upper bound for n up to ~10000 */

static seq_t cache[CACHE_MAX];
static size_t cached = 0;    /* number of terms already stored */
static seq_t last_checked = 0;  /* last integer examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0; safeguard */

    /* generate terms until we have enough */
    while (cached < n) {
        last_checked++;

        /* count runs in base 3 */
        seq_t x = last_checked;
        int prev = -1;
        int runs = 0;

        while (x) {
            int d = (int)(x % 3);
            if (d != prev) {
                runs++;
                prev = d;
            }
            x /= 3;
        }

        if (runs % 9 == 1) {
            assert(cached < CACHE_MAX);
            cache[cached] = last_checked;
            cached++;
        }
    }

    return cache[n-1];
}
