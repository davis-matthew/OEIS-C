/*
 * A002516  Earliest sequence with a(a(n)) = 2n.
 * https://oeis.org/A002516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(4n) = 2*a(2n), a(4n+1) = 4n+3, a(4n+2) = 2*a(2n+1), a(4n+3) =
 *            8n+2
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    idx_t k = n / 4;
    idx_t mod = n % 4;
    
    if (mod == 0) {
        return 2 * a(2 * k);
    } else if (mod == 1) {
        return 4 * (seq_t)k + 3;
    } else if (mod == 2) {
        return 2 * a(2 * k + 1);
    } else {
        return 8 * (seq_t)k + 2;
    }
}
