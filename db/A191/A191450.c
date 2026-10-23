/*
 * A191450  Dispersion of (3*n-1), read by antidiagonals.
 * https://oeis.org/A191450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   This function implements the dispersion of the sequence 3*n-1,
 *            read by antidiagonals. The dispersion array D has D(n, k) =
 *            s^{k-1}(t(n)) where s(x) = 3*x-1 and t is the increasing
 *            sequence of positive integers not congruent to 2 mod 3 (t(1)=1).
 *            The one‑dimensional sequence a(n) is obtained by rea...
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 231, u64 n <= 862,
 *            u128 n <= 3322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: smallest m with m*(m+1)/2 >= n */
static idx_t diagonal_from_index(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but return 0 */

    /* Determine number of rows needed */
    idx_t max_row = diagonal_from_index(n);

    /* Allocate current values for each row (1‑based) */
    seq_t *cur = (seq_t *)calloc((size_t)(max_row + 1), sizeof(seq_t));
    if (!cur)
        return (seq_t)0;   /* allocation failure */

    /* Initialise each row with t(i) = numbers not ≡ 2 (mod 3) */
    for (idx_t i = 1; i <= max_row; ++i) {
        idx_t w = (i - 1) >> 1;        /* floor((i-1)/2) */
        idx_t r = (i - 1) & 1;         /* (i-1) mod 2 */
        idx_t t_val = 3 * w + (r ? 3 : 1);
        cur[i] = (seq_t)t_val;
    }

    /* Traverse antidiagonals and output the requested term */
    idx_t count = 0;
    for (idx_t d = 1; d <= max_row; ++d) {
        for (idx_t r = 1; r <= d; ++r) {
            ++count;
            seq_t val = cur[r];
            if (count == n) {
                free(cur);
                return val;
            }
            /* advance the row value: s(x) = 3*x - 1 */
            cur[r] = cur[r] * (seq_t)3 - (seq_t)1;
        }
    }

    /* Fallback (should never be reached) */
    seq_t result = cur[1];
    free(cur);
    return result;
}
