/*
 * A080041  a(n) = floor((1+sqrt(3))^n).
 * https://oeis.org/A080041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((1+sqrt(3))^n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for the sequence S(n) = (1+sqrt(3))^n + (1-sqrt(3))^n */
    static seq_t *S = NULL;
    static idx_t len = 0;  /* number of values currently cached (≥0) */

    /* Ensure at least the first two terms are available */
    if (len == 0) {
        /* allocate space for S[0] and S[1] */
        S = (seq_t *)realloc(S, 2 * sizeof(seq_t));
        S[0] = (seq_t)2;  /* S(0) = 2 */
        S[1] = (seq_t)2;  /* S(1) = 2 */
        len  = 2;
    }

    /* Extend the cache up to index n if needed */
    if (n >= len) {
        size_t new_len = (size_t)(n + 1);
        S = (seq_t *)realloc(S, new_len * sizeof(seq_t));

        seq_t two = (seq_t)2;
        for (idx_t i = len; i <= n; ++i) {
            S[i] = two * S[i - 1] + two * S[i - 2];
        }
        len = (idx_t)new_len;
    }

    /* a(n) = S(n) - 1 for even n, S(n) for odd n */
    seq_t result = S[n];
    if ((n & 1) == 0) {
        result = result - (seq_t)1;
    }
    return result;
}
