/*
 * A059021  Number of 3 X n checkerboards (with at least one red square) in
 *          which the set of red squares is edge-connected.
 * https://oeis.org/A059021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059021:
   a(n) = 9*a(n-1) - 26*a(n-2) + 35*a(n-3) - 22*a(n-4)
          - 3*a(n-5) + 16*a(n-6) - 9*a(n-7) + a(n-8).
   Initial values:
   a(0)=0, a(1)=6, a(2)=40, a(3)=218, a(4)=1126, a(5)=5726,
   a(6)=28992, a(7)=146642.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *data = NULL;
    static idx_t len   = 0;   /* number of computed entries */
    static idx_t cap   = 0;   /* allocated capacity */

    /* helper: ensure capacity at least new_cap */
    if (cap < n + 1) {
        idx_t new_cap = (cap == 0) ? 8 : cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        /* allocate new array */
        seq_t *tmp = (seq_t *)realloc(data, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;          /* out of memory */
        data = tmp;
        cap = new_cap;
    }

    /* initialise the first 8 terms lazily */
    if (len == 0) {
        data[0] = (seq_t)0;
        data[1] = (seq_t)6;
        data[2] = (seq_t)40;
        data[3] = (seq_t)218;
        data[4] = (seq_t)1126;
        data[5] = (seq_t)5726;
        data[6] = (seq_t)28992;
        data[7] = (seq_t)146642;
        len   = 8;
    }

    /* if requested value already computed, return it */
    if (n < len) return data[n];

    /* compute values up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 8) continue; /* initialised above */

        seq_t v = (seq_t)9   * data[i-1]
                - (seq_t)26  * data[i-2]
                + (seq_t)35  * data[i-3]
                - (seq_t)22  * data[i-4]
                - (seq_t)3   * data[i-5]
                + (seq_t)16  * data[i-6]
                - (seq_t)9   * data[i-7]
                +             data[i-8];
        data[i] = v;
    }

    len = n + 1;
    return data[n];
}
