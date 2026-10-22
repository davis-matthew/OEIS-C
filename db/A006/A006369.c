/*
 * A006369  a(n) = 2*n/3 for n divisible by 3, otherwise a(n) = round(4*n/3).
 *          Or, equivalently, a(3*n-2) = 4*n-3, a(3*n-1) = 4*n-1, a(3*n) =
 *          2*n.
 * https://oeis.org/A006369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(3*n) = 2*n, a(3*n+1) = 4*n+1, a(3*n-1) = 4*n-1
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    idx_t r = n % 3;
    
    if (r == 0) {
        return sn * 2 / 3;
    } else if (r == 1) {
        return (sn * 4 - 1) / 3;
    } else {  // r == 2
        return (sn * 4 + 1) / 3;
    }
}
