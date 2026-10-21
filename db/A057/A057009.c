/*
 * A057009  Number of conjugacy classes of subgroups of index 3 in free group
 *          of rank n.
 * https://oeis.org/A057009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(n-1)+3^(n-1)-2^(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses simple repeated multiplication, which is fast enough for
   n up to about 10,000.  For larger n the result grows too big for a
   fixed-width seq_t and the build will provide an arbitrary-precision
   implementation.  The algorithm works correctly when all intermediate
   values fit in seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) /* safety: sequence is defined for n≥1 */
        return 0;

    idx_t e = n - 1;          /* exponent (n‑1) */
    seq_t p6 = 1, p3 = 1, p2 = 1;  /* 6^e, 3^e, 2^e */

    for (idx_t i = 0; i < e; ++i) {
        p6 *= (seq_t)6;
        p3 *= (seq_t)3;
        p2 *= (seq_t)2;
    }

    return p6 + p3 - p2;
}
