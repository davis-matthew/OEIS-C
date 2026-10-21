/*
 * A064412  At stage 1, start with a unit equilateral equiangular triangle. At
 *          each successive stage add 3*(n-1) new triangles around outside
 *          with edge-to-edge contacts. Sequence gives number of triangles
 *          (regardless of size) at n-th stage.
 * https://oeis.org/A064412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k+1) = (7k³ + 12k² + 7k + 2) / 2 a(2k) = (28k³ + 6k² + 4k + 1
 *            + (-1)^(k+1)) / 8 (k = 0,1,2,…, n = 1,2,3,4,…)
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* n >= 1 */
    if ((nn & 1) != 0) {           /* odd n : n = 2k+1 */
        seq_t k   = (nn - 1) >> 1;
        seq_t k2  = k * k;
        seq_t k3  = k2 * k;
        seq_t num = 7 * k3 + 12 * k2 + 7 * k + 2;
        return num >> 1;          /* divide by 2 */
    } else {                       /* even n : n = 2k */
        seq_t k   = nn >> 1;
        seq_t k2  = k * k;
        seq_t k3  = k2 * k;
        seq_t num = 28 * k3 + 6 * k2 + 4 * k + 1; /* base part */
        /* add (-1)^(k+1) */
        int  sign = ((k + 1) & 1) ? -1 : 1; /* odd -> -1, even -> +1 */
        if (sign == 1) {
            num += 1;
        } else {
            num -= 1;
        }
        return num >> 3;          /* divide by 8 */
    }
}
