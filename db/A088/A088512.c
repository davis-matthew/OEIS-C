/*
 * A088512  Number of partitions of n into two parts whose xor-sum is n.
 * https://oeis.org/A088512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0)=0, a(1)=0, a(2*n)=a(n), a(2*n+1)=2*a(n)+1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    while (n >= 2) {
        if (n & 1) {
            ans = ans * 2 + 1;
        }
        n >>= 1;
    }
    return ans;
}
