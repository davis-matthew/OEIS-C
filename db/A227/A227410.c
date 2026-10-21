/*
 * A227410  Palindromic prime numbers representing a date in "condensed
 *          American notation" MMDDYY.
 * https://oeis.org/A227410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   a(n) is the n‑th palindromic prime that can be read as a date in
 *            the condensed American notation MMDDYY. The dates are restricted
 *            to non‑leap years (February always 28 days). The function
 *            generates all such dates, filters palindromic primes, sorts
 *            them, and returns the (n‑1)th element of this lis...
 * Range:     u8 none, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   cmp_seq_t, ensure_capacity, is_palindrome, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/is_prime_small.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* Cache of all palindromic prime dates */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    if (sz == 0) {
        /* Build list on first call */
        for (idx_t yy = 0; yy < 100; ++yy) {          /* year 00–99   */
            for (idx_t mm = 1; mm <= 12; ++mm) {      /* month 01–12 */
                idx_t days;
                switch (mm) {
                    case 2: days = 28; break;          /* February non‑leap */
                    case 4: case 6: case 9: case 11: days = 30; break;
                    default: days = 31; break;
                }
                for (idx_t dd = 1; dd <= days; ++dd) { /* day 01…days */
                    seq_t val = (seq_t)mm * 10000
                              + (seq_t)dd * 100
                              + (seq_t)yy;              /* MMDDYY */
                    if (is_palindrome(val, 10) &&
                        is_prime_small((idx_t)val)) {
                        if (sz == cap) {
                            size_t ncap = cap == 0 ? 8 : cap * 2;
                            ensure_capacity(ncap, &cap, (void **)&cache,
                                            sizeof(seq_t));
                        }
                        cache[sz++] = val;
                    }
                }
            }
        }
        /* Sort in ascending order */
        qsort(cache, sz, sizeof(seq_t), (int(*)(const void*, const void*))cmp_seq_t);
    }

    if (n == 0 || n > sz) return 0;   /* undefined beyond the list size */
    return cache[n - 1];
}
