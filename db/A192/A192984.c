/*
 * A192984  In repeated iterations of function m->m/3 if m == 0 (mod 3),
 *          m->4m+a if 4m+a == 0 (mod 3) where a = 1 or 2, a(n) is maximum
 *          value achieved if starting from n.
 * https://oeis.org/A192984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the maximum value reached while iterating the map
 * m -> m/3   if m ≡ 0 (mod 3),
 *         m -> 4m+2 if m ≡ 1 (mod 3),
 *         m -> 4m+1 if m ≡ 2 (mod 3),
 * until a value repeats (the first repeated value ends the run).   */

#include <stdbool.h>

static inline seq_t next_val(seq_t x) {
    if ((x % 3) == 0)
        return x / 3;
    return 4 * x + ((x % 3 == 1) ? 2 : 1);
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    seq_t start = (seq_t)n;
    seq_t max_val = start;

    /* tortoise–hare cycle detection (Floyd) with maximum tracking */
    seq_t tortoise = start;
    seq_t hare = next_val(start);
    if (hare > max_val) max_val = hare;

    while (tortoise != hare) {
        /* move tortoise one step */
        tortoise = next_val(tortoise);
        if (tortoise > max_val) max_val = tortoise;

        /* move hare two steps, updating max for both intermediate values */
        hare = next_val(hare);
        if (hare > max_val) max_val = hare;
        hare = next_val(hare);
        if (hare > max_val) max_val = hare;
    }

    return max_val;
}
