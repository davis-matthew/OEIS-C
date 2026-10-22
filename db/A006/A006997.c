/*
 * A006997  Partitioning integers to avoid arithmetic progressions of length
 *          3.
 * https://oeis.org/A006997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n+k) = floor((3*a(n)+k)/2), 0 <= k <= 2; a(0)=0, a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;
    
    idx_t m = n / 3;
    idx_t k = n % 3;
    seq_t a_m = a(m);
    return ((seq_t)3 * a_m + (seq_t)k) / (seq_t)2;
}
