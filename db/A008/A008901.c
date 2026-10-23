/*
 * A008901  x->x/2 if x even, x->3x-1 if x odd.
 * https://oeis.org/A008901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=90; a(n+1)= a(n)/2 if a(n) even else 3*a(n)-1
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 90;
    for (idx_t i = 0; i < n; ++i) {
        if ((x & 1) == 0) {
            x = x / 2;
        } else {
            x = 3 * x - 1;
        }
    }
    return x;
}
