/*
 * A038129  Beatty sequence for cube root of 2.
 * https://oeis.org/A038129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*∛2)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 2378, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes floor(cuberoot(x)) for non-negative x.
   Uses binary search and avoids overflow by checking before
   each multiplication. */
static seq_t floor_cube_root(seq_t x)
{
    seq_t low = 0, high = x;

    while (low <= high) {
        seq_t mid = low + (high - low) / 2;

        /* check if mid^3 <= x */
        seq_t prod = mid;
        bool ok = true;

        if (mid != 0) {
            /* mid^2 */
            if (prod > x / mid) { ok = false; }
            else { prod *= mid; }

            /* mid^3 */
            if (ok && mid != 0 && prod > x / mid) { ok = false; }
            else if (ok) { prod *= mid; }
        }

        if (ok && prod <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Beatty sequence a(n) = floor(n*∛2) for n >= 0 */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t cube   = n_seq * n_seq * n_seq;   /* n^3 */
    seq_t val    = cube * (seq_t)2;          /* 2*n^3 */
    return floor_cube_root(val);
}
