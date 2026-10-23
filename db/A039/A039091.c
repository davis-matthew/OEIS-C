/*
 * A039091  Numbers whose base-9 representation has the same number of 2's and
 *          3's.
 * https://oeis.org/A039091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A039091
 *  Numbers whose base-9 representation has the same number of 2's and 3's.
 *
 *  The first term (offset 1) is 0.  For each n ≥ 1 this function returns the
 *  n‑th such integer (sorted increasingly).  The implementation scans
 *  successive integers, tests the digit condition and stores the results
 *  in a static array so that each value is computed only once.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache   = NULL;   /* array of found terms */
    static size_t  cap    = 0;      /* allocated capacity      */
    static idx_t   next   = 0;      /* next integer to test    */
    static size_t  len    = 0;      /* number of terms stored  */

    if (n == 0)   /* undefined for n < 1, but we return 0 for safety */
        return (seq_t)0;

    while (len < n) {
        /* test whether 'next' satisfies the property */
        seq_t  val   = (seq_t)next;
        unsigned int c2 = 0, c3 = 0;
        seq_t  tmp   = val;
        while (tmp) {
            seq_t d = tmp % 9;
            if (d == 2) ++c2;
            else if (d == 3) ++c3;
            tmp /= 9;
        }
        if (c2 == c3) {
            /* ensure room in the cache */
            if (len == cap) {
                size_t newcap = (cap == 0) ? 16 : cap << 1;
                seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newp) return (seq_t)0;      /* allocation failed */
                cache = newp;
                cap   = newcap;
            }
            cache[len++] = val;
        }
        ++next;
    }

    return cache[n - 1];
}
