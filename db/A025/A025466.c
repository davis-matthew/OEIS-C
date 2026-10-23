/*
 * A025466  Number of partitions of n into 4 distinct nonnegative cubes.
 * https://oeis.org/A025466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 distinct nonnegative
 *            cubes w > x > y > z >= 0 (so w^3 + x^3 + y^3 + z^3 = n)
 * Range:     u8 n <= 1441, u16 n <= 9526, u32 n <= 9526, u64 n <= 9526,
 *            u128 n <= 9526, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation is a straightforward brute‑force enumeration of all
   quadruples w > x > y > z with the cube root of n as an upper bound.
   For n up to about 10000 the cube root is only 22, so the 4‑nested
   loops run at most ~10⁴ iterations, far below the time limit. */

seq_t a(idx_t n) {
    seq_t count = 0;

    /* Compute floor(cuberoot(n)) as an integer bound for the loops. */
    idx_t max = 0;
    while ((max + 1) * (max + 1) * (max + 1) <= n) ++max;

    /* enumerate quadruples w > x > y > z >= 0 */
    for (idx_t w = 0; w <= max; ++w) {
        seq_t wc = (seq_t)w * w * w;
        for (idx_t x = 0; x < w; ++x) {
            seq_t xc = (seq_t)x * x * x;
            for (idx_t y = 0; y < x; ++y) {
                seq_t yc = (seq_t)y * y * y;
                for (idx_t z = 0; z < y; ++z) {
                    seq_t sum = wc + xc + yc + (seq_t)z * z * z;
                    if (sum == (seq_t)n) ++count;
                }
            }
        }
    }
    return count;
}
