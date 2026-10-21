/*
 * A075683  2nd differences of A075681.
 * https://oeis.org/A075683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 10; annotations: a(1)=2, a(2)=17, a(3)=20
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Not used (offset is 1) */
    seq_t res = (seq_t)n * 3 + 10;   /* Base formula for n ≥ 4 */
    if (n == 1)
        res -= 11;                  /* 3*1+10-11 = 2  */
    else if (n == 2)
        res += 1;                   /* 3*2+10+1  = 17 */
    else if (n == 3)
        res += 1;                   /* 3*3+10+1  = 20 */
    return res;
}
