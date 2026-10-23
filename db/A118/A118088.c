/*
 * A118088  a(0) = 0, a(n) = (1+2*0^(n mod 3))*a(floor(n/3)) + n mod 3.
 * https://oeis.org/A118088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(3*n) = 3*a(n); a(3*n+1) = a(n)+1; a(3*n+2) = a(n)+2;
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    idx_t m = n / 3;
    unsigned int r = n % 3;
    seq_t res = a(m);
    if (r == 0) {
        return res * (seq_t)3;
    } else {
        return res + (seq_t)r;
    }
}
