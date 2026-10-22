/*
 * A004161  Tetrahedral numbers written backwards.
 * https://oeis.org/A004161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = reverse_digits(n*(n+1)*(n+2)/6)
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 1815, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t tet = nn * (nn + 1) * (nn + 2) / 6;
    
    seq_t result = 0;
    while (tet > 0) {
        result = result * 10 + (tet % 10);
        tet /= 10;
    }
    
    return result;
}
