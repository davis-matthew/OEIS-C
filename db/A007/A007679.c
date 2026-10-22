/*
 * A007679  If n mod 4 = 0 then 2^(n-1)+1 elif n mod 4 = 2 then 2^(n-1)-1 else
 *          2^(n-1).
 * https://oeis.org/A007679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n mod 4 = 0 then 2^(n-1)+1 elif n mod 4 = 2 then 2^(n-1)-1
 *            else 2^(n-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2_part = (seq_t)1 << (n - 1);
    idx_t remainder = n % 4;
    
    if (remainder == 0) {
        return pow2_part + 1;
    } else if (remainder == 2) {
        return pow2_part - 1;
    } else {
        return pow2_part;
    }
}
