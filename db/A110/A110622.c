/*
 * A110622  n^2 followed by n followed by n^3 followed by n^4.
 * https://oeis.org/A110622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * if n = 4m + 1 -> (m+1)^2 * if n = 4m + 2 -> (m+1) * if
 *            n = 4m + 3 -> (m+1)^3 * if n = 4m + 4 -> (m+1)^4 * where m =
 *            ⌊(n-1)/4⌋
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1023, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n=0, guard */
    idx_t m = (n - 1) / 4;                   /* floor((n-1)/4) */
    seq_t base = (seq_t)(m + 1);             /* (m+1) as seq_t */
    idx_t r = (n - 1) & 3;                   /* (n-1) mod 4 */
    seq_t ans;
    switch (r) {
        case 0:   ans = base * base;            break;   /* (m+1)^2   */
        case 1:   ans = base;                   break;   /* (m+1)     */
        case 2:   ans = base * base * base;     break;   /* (m+1)^3   */
        case 3:   ans = base * base * base * base; break;/* (m+1)^4   */
    }
    return ans;
}
