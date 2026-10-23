/*
 * A045809  9-ish numbers (end in 13, 37, 59, 91).
 * https://oeis.org/A045809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 100*floor((n-1)/4) + {13,37,59,91}((n-1) mod 4)
 *            (1-indexed)
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0, return 0 as a safe fallback */
        return 0;
    /* 100 * floor((n-1)/4) + offset[ (n-1) % 4 ]  */
    static const seq_t offset[4] = {13, 37, 59, 91};

    idx_t block = (n - 1) / 4;   /* number of completed 100‑blocks */
    idx_t pos   = (n - 1) % 4;   /* position within the block */

    return (seq_t)block * 100 + offset[pos];
}
