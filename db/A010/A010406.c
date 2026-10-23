/*
 * A010406  Squares mod 45.
 * https://oeis.org/A010406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* a(n) is the n-th non‑negative integer whose remainder modulo 45 is a
   quadratic residue modulo 45 (i.e. congruent to a perfect square modulo 45). */
static const unsigned char cum[45] = {
    1, 2, 2, 2, 3, 3, 3, 3, 3, 4,
    5, 5, 5, 5, 5, 5, 6, 6, 6, 7,
    7, 7, 7, 7, 7, 8, 8, 8, 8, 8,
    8, 9, 9, 9,10,10,11,11,11,11,
   12,12,12,12,12
};

static inline seq_t count_leq(seq_t k)
{
    seq_t block = k / 45;
    seq_t rem   = k % 45;
    return block * 12 + cum[(size_t)rem];
}

seq_t a(idx_t n)
{
    /* Handle n == 0 (not part of the offset but harmless). */
    if (n == 0)
        return 0;

    /* Find an upper bound h such that count_leq(h) >= n. */
    seq_t h = 1;
    while (count_leq(h) < n) {
        seq_t next = h * 2;
        /* Stop if the multiplication would overflow the type. */
        if (next <= h)
            break;
        h = next;
    }

    /* Binary search for the smallest k with count_leq(k) >= n. */
    seq_t lo = 0;
    while (lo < h) {
        seq_t mid = lo + ((h - lo) >> 1);
        if (count_leq(mid) >= n)
            h = mid;
        else
            lo = mid + 1;
    }
    return lo;   /* lo == h */
}
