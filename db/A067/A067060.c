/*
 * A067060  A permutation of the positive integers in groups of four such that
 *          any two consecutive numbers differ by at least 2.
 * https://oeis.org/A067060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*floor((n-1)/4) + offset, where offset = 3 if
 *            (n-1)%4==0, 1 if (n-1)%4==1, 4 if (n-1)%4==2, 2 if (n-1)%4==3
 *            [OEIS A067060]
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    idx_t m = (n - 1) / 4;      /* the block number (0‑based) */
    idx_t r = (n - 1) % 4;      /* position within the block (0‑3) */
    seq_t base = (seq_t)4 * m;  /* common part of the answer */

    switch (r) {
        case 0: return base + (seq_t)3;  /* a(4m+1) = 4m+3   */
        case 1: return base + (seq_t)1;  /* a(4m+2) = 4m+1   */
        case 2: return base + (seq_t)4;  /* a(4m+3) = 4m+4   */
        case 3: return base + (seq_t)2;  /* a(4m+4) = 4m+2   */
        default: return (seq_t)0;       /* unreachable */
    }
}
