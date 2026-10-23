/*
 * A041759  Denominators of continued fraction convergents to sqrt(399).
 * https://oeis.org/A041759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 1, a(2) = 39, a(3) = 40,
   a(n) = 40*a(n-2) - a(n-4)      for n > 3. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t memo_size = 0;

    size_t needed = (size_t)n + 1;
    if (needed > memo_size) {
        seq_t *tmp = (seq_t *)realloc(memo, needed * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        for (size_t i = memo_size; i < needed; ++i) {
            if (i < 4) {
                switch (i) {
                    case 0: memo[i] = 1; break;
                    case 1: memo[i] = 1; break;
                    case 2: memo[i] = 39; break;
                    case 3: memo[i] = 40; break;
                }
            } else {
                memo[i] = 40 * memo[i - 2] - memo[i - 4];
            }
        }
        memo_size = needed;
    }
    return memo[(size_t)n];
}
