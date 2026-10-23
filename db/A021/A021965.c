/*
 * A021965  Decimal expansion of 1/961.
 * https://oeis.org/A021965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of the expansion of 1/961 (0‑based
 *            indexing)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t   *digits = NULL;   /* array of digits (0‑9) */
    static size_t   cache_len = 0;   /* number of digits stored */
    static size_t   cache_cap = 0;   /* allocated capacity */
    static uint64_t rem = 1;         /* current remainder in division by 961 */

    /* extend cache up to n (inclusive) */
    if (cache_len <= (size_t)n) {
        /* compute new required length */
        size_t new_len = (size_t)n + 1;
        if (cache_cap < new_len) {
            /* enlarge capacity: double until enough */
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < new_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure: abort or return 0 */
                abort(); /* safe abort; in contest setting should not happen */
            }
            digits = tmp;
            cache_cap = new_cap;
        }

        /* compute digits from current cache_len up to new_len-1 */
        for (size_t i = cache_len; i < new_len; ++i) {
            rem *= 10ULL;          /* bring next digit into place */
            seq_t digit = (seq_t)(rem / 961ULL);   /* integer division */
            rem     = rem % 961ULL;              /* remainder for next step */
            digits[i] = digit;
        }
        cache_len = new_len;
    }

    return digits[(size_t)n];
}
