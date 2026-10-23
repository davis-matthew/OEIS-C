/*
 * A159653  Numerator of Hermite(n, 15/19).
 * https://oeis.org/A159653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 30*a(n-1) - 722*(n-1)*a(n-2); a(0)=1, a(1)=30
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Adjust this limit if the maximum required n exceeds it.  */
#define MAX_N 200000

static seq_t cache[MAX_N + 1];
static int current_len = -1;  /* -1 means nothing initialized yet */

seq_t a(idx_t n) {
    /* Initialize base cases once. */
    if (current_len == -1) {
        cache[0] = (seq_t)1;
        if (MAX_N >= 1) cache[1] = (seq_t)30;
        current_len = n < 1 ? (int)n : 1;   /* current_len = min(n,1) after init */
        /* If n is 0 or 1, return immediately. */
        if (n <= 1) return cache[(idx_t)current_len];
    }
    /* If requested n is beyond the current cache, extend it. */
    if ((idx_t)current_len < n) {
        idx_t i;
        for (i = (idx_t)current_len + 1; i <= n; ++i) {
            idx_t i_minus_1 = i - 1;
            seq_t term1 = (seq_t)30 * cache[i_minus_1];
            seq_t term2 = (seq_t)722 * (seq_t)i_minus_1 * cache[i - 2];
            cache[i] = term1 - term2;
        }
        current_len = (int)n;
    }
    return cache[n];
}
