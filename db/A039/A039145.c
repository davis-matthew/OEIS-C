/*
 * A039145  Numbers whose base-10 representation has the same number of 4's
 *          and 9's.
 * https://oeis.org/A039145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth non‑negative integer whose decimal representation
   contains an equal number of 4’s and 9’s.  The sequence begins
   0,1,2,3,5,6,7,8,10,11,12,13,15,16,17,18,20,21,22,23,25,26,27,28,... */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the already generated terms and for the current
       candidate number being tested                    */
    static seq_t *cache = NULL;   /* array of sequence terms          */
    static size_t  cap   = 0;      /* allocated capacity of cache      */
    static size_t  len   = 0;      /* number of terms currently in cache */
    static idx_t   next  = 1;      /* next decimal number to test      */

    /* initialise on first call */
    if (len == 0) {
        cap = 1024;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;  /* a(1) = 0 */
        len   = 1;
        next  = 1;            /* start testing from 1            */
    }

    /* generate further terms until we have at least n of them */
    while (len < n) {
        idx_t num = next;
        while (1) {
            /* count 4’s and 9’s in decimal representation of num */
            idx_t t = num;
            int cnt4 = 0, cnt9 = 0;
            while (t > 0) {
                int d = t % 10;
                if (d == 4) cnt4++;
                else if (d == 9) cnt9++;
                t /= 10;
            }
            if (cnt4 == cnt9) {              /* num qualifies        */
                if (len == cap) {            /* enlarge cache if needed */
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = (seq_t)num;    /* store term            */
                next = num + 1;               /* next candidate to try */
                break;                         /* move on to next term  */
            }
            num++;                            /* try next number       */
        }
    }

    return cache[n-1];                       /* a(1) is at cache[0]   */
}
