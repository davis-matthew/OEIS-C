/*
 * A039255  Numbers whose base-12 representation has the same number of 5's
 *          and 7's.
 * https://oeis.org/A039255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 178, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers whose base‑12 representation contains an equal number
   of 5’s and 7’s.  The n-th such number (with offset 1) is returned.
   The algorithm simply scans integers, counting 5’s and 7’s in base 12
   until n terms have been found.  The results are cached for
   subsequent calls. */

#include <stdlib.h>
#include <stdint.h>   /* for uint64_t used only in a cast */

seq_t a(idx_t n)
{
    /* Cache of terms already found */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocation size of cache        */
    static size_t len  = 0;   /* number of terms stored          */
    static seq_t next_start = 0; /* next integer to examine      */

    /* Allocate the cache once */
    if (!cache) {
        cap = 1024;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        seq_t num = next_start;   /* start from where we left off */

        /* Search for the next qualifying number */
        for (;;) {
            seq_t temp = num;
            unsigned count5 = 0, count7 = 0;   /* counts of 5 and 7   */

            /* Examine digits of num in base 12 */
            while (temp != 0) {
                /* remainder < 12, safe to cast to uint64_t */
                uint64_t digit = (uint64_t)(temp % 12);
                if (digit == 5) ++count5;
                else if (digit == 7) ++count7;
                temp /= 12;
            }

            if (count5 == count7) {          /* qualifies */
                /* Grow cache if necessary */
                if (len >= cap) {
                    size_t newcap = cap * 2;
                    seq_t *ptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!ptr) { /* allocation failure – abort silently */
                        /* In a real project you might handle this differently */
                        exit(EXIT_FAILURE);
                    }
                    cache = ptr;
                    cap   = newcap;
                }
                cache[len++] = num;           /* store the term           */
                next_start = num + 1;         /* next candidate to check */
                break;                       /* move on to next term     */
            }
            ++num; /* try the next integer */
        }
    }

    return cache[n-1];
}
