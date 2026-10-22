/*
 * A003278  Szekeres's sequence: a(n)-1 in ternary = n-1 in binary; also: a(1)
 *          = 1, a(2) = 2, and thereafter a(n) is smallest number k which
 *          avoids any 3-term arithmetic progression in a(1), a(2), ...,
 *          a(n-1), k.
 * https://oeis.org/A003278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n)-1 in ternary = n-1 in binary
 * Range:     u8 n <= 38, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 0;
    seq_t power = 1;
    
    while (m > 0) {
        if (m & 1) {
            result = result + power;
        }
        power = power * 3;
        m = m >> 1;
    }
    
    return result + 1;
}
