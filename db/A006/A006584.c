/*
 * A006584  If n mod 2 = 0 then n*(n^2-4)/12 else n*(n^2-1)/12.
 * https://oeis.org/A006584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2-4)/12 if n is even, n*(n^2-1)/12 if n is odd
 * Range:     u8 n <= 14, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    
    if (n & 1) {  /* n is odd */
        return sn * (n2 - 1) / 12;
    } else {      /* n is even */
        return sn * (n2 - 4) / 12;
    }
}
