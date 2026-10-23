/*
 * A036292  a(n) = n*6^n.
 * https://oeis.org/A036292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*6^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Compute a(n) = n * 6^n.
 * The function caches powers of 6 to allow repeated calls with
 * arbitrary order of n values.  It works for any integer type
 * (uint8_t ... uint128_t or a big–integer class) as long as
 * the basic arithmetic operators (+, -, *, /) are defined.
 */
seq_t a(idx_t n)
{
    /* Static cache of powers of 6: pow6[i] = 6^i */
    static seq_t *pow6 = NULL;
    static size_t pow6_len = 0;   /* number of elements stored */

    /* Ensure the cache contains 6^i up to i = n */
    if (pow6_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(pow6, new_len * sizeof(seq_t));
        if (!tmp) abort();      /* out of memory */
        pow6 = tmp;

        /* Compute new entries */
        for (size_t i = pow6_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                pow6[0] = (seq_t)1;           /* 6^0 */
            } else {
                pow6[i] = pow6[i - 1] * (seq_t)6;   /* 6^i = 6^(i-1) * 6 */
            }
        }
        pow6_len = new_len;
    }

    /* a(n) = n * 6^n */
    return pow6[(size_t)n] * (seq_t)n;
}
