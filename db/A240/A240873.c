/*
 * A240873  Number of partitions p of n into distinct parts such that max(p) =
 *          5 + min(p).
 * https://oeis.org/A240873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the number of partitions into distinct parts with
   max(p) = 5 + min(p).  For each subset of the four possible middle
   parts we check whether a partition exists. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t mask = 0; mask < 16; ++mask) {
        idx_t sumK = 0;
        idx_t r = 0;
        for (idx_t i = 0; i < 4; ++i) {
            if (mask & ((idx_t)1 << i)) {
                ++r;
                sumK += (idx_t)(i + 1);
            }
        }
        if (n < 5 + sumK)
            continue;
        idx_t numerator = n - 5 - sumK;
        idx_t denom = 2 + r;
        if (numerator >= denom && (numerator % denom) == 0)
            ++count;
    }
    return count;
}
