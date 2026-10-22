/*
 * A006185  Number of pair-coverings with largest block size 3.
 * https://oeis.org/A006185
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/6 if n ≡ 1,3 (mod 6); a(n) = n*(n+1)/6 if n ≡ 0,2
 *            (mod 6); a(n) = (n^2+n+4)/6 if n ≡ 4 (mod 6); a(n) =
 *            (n^2-n+16)/6 if n ≡ 5 (mod 6)
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    unsigned int mod = n % 6;
    
    switch (mod) {
        case 0:
        case 2:
            return nn * (nn + 1) / 6;
        case 1:
        case 3:
            return nn * (nn - 1) / 6;
        case 4:
            return (nn * nn + nn + 4) / 6;
        case 5:
            return (nn * nn - nn + 16) / 6;
    }
    return 0;
}
