/*
 * A024151  a(n) = 12^n - n^11.
 * https://oeis.org/A024151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 17, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(n) for n ≥ 0
   a(n) = 12^n - n^11
*/
seq_t a(idx_t n)
{
    /* Cache of powers of 12: pow12[i] = 12^i */
    static seq_t *pow12 = NULL;
    static size_t pow12_len = 0;   /* current number of cached values (i.e., max index + 1) */
    static size_t pow12_cap = 0;   /* allocated capacity */

    /* Initialise cache */
    if (pow12_len == 0) {
        pow12_cap = 16;
        pow12 = (seq_t *)malloc(pow12_cap * sizeof(seq_t));
        if (!pow12) return 0;          /* memory allocation failure – unlikely for contest */
        pow12[0] = 1;                  /* 12^0 */
        pow12_len = 1;
    }

    /* Extend cache up to n if needed */
    if ((size_t)n >= pow12_len) {
        size_t needed = (size_t)n + 1;
        if (pow12_cap < needed) {
            size_t new_cap = pow12_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(pow12, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            pow12 = tmp;
            pow12_cap = new_cap;
        }
        for (size_t i = pow12_len; i < needed; ++i) {
            pow12[i] = pow12[i - 1] * (seq_t)12;
        }
        pow12_len = needed;
    }

    /* Compute n^11 */
    seq_t n_val = (seq_t)n;
    seq_t n_pow11 = 1;
    for (int i = 0; i < 11; ++i)
        n_pow11 *= n_val;

    /* Return 12^n - n^11 */
    return pow12[n] - n_pow11;
}
