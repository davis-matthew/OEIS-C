/*
 * A185061  Position of the first occurrence of n in A193358 when it is
 *          considered to have the starting offset 1 instead of 0.
 * https://oeis.org/A185061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = n*(n-1)+1 (n>1); a(2n+1) = n*(n+3)+1 Special cases:
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n & (idx_t)1) {        /* odd index: n = 2k+1 */
        idx_t k = (n - 1) / 2;
        seq_t kk = (seq_t)k;
        return kk * (kk + 3) + (seq_t)1;
    } else {                   /* even index: n = 2k */
        idx_t k = n / 2;
        seq_t kk = (seq_t)k;
        return kk * (kk - 1) + (seq_t)1;
    }
}
