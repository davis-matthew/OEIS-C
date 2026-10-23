/*
 * A164472  Number of binary strings of length n with no substrings equal to
 *          0001 0101 or 1010.
 * https://oeis.org/A164472
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 87, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164472: Number of binary strings of length n with no substrings
    equal to 0001, 0101 or 1010.
    Recurrence (from the rational generating function):
        a(n) = a(n-1) + 2*a(n-3) - a(n-6)   for n >= 10
    Initial values (offset 4):
        a(4) = 13
        a(5) = 22
        a(6) = 37
        a(7) = 61
        a(8) = 101
        a(9) = 167
    This implementation memoizes the values in a static dynamic array.
    It works for any index n >= 4, in any call order.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* undefined for n < 4 */

    /* Convert the index n to the offset in the memoization array */
    idx_t idx = n - 4;            /* element 0 corresponds to a(4) */

    /* Static memoization state */
    static seq_t *cache = NULL;   /* array of precomputed terms          */
    static size_t  cap   = 0;     /* allocated capacity                   */
    static size_t  sz    = 0;     /* number of terms currently stored     */

    /* Initial allocation if necessary */
    if (!cache) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure → undefined value */
        sz = 0;
    }

    /* Grow the cache if needed */
    if ((size_t)idx >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure → undefined value */
        cache = tmp;
        cap = new_cap;
    }

    /* Predefined initial values for the first six terms */
    static const seq_t init[6] = {13, 22, 37, 61, 101, 167};

    /* Compute terms up to the requested index */
    for (size_t i = sz; i <= (size_t)idx; ++i) {
        if (i < 6) {                      /* fill from the fixed table   */
            cache[i] = init[i];
        } else {                          /* recurrence for i >= 6      */
            seq_t term = cache[i-1] + (cache[i-3] * (seq_t)2) - cache[i-6];
            cache[i] = term;
        }
    }
    sz = idx + 1;                          /* now we have terms up to idx */

    return cache[idx];
}
