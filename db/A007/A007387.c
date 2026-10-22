/*
 * A007387  Number of 3rd-order maximal independent sets in cycle graph.
 * https://oeis.org/A007387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-5) for n >= 9, with base cases a(1)=0,
 *            a(2)=2, a(3)=3, a(4)=2, a(5)=5, a(6)=2, a(7)=7, a(8)=2
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 104, u64 n <= 208,
 *            u128 n <= 417, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base[9] = {0, 0, 2, 3, 2, 5, 2, 7, 2};
    
    if (n < 9) {
        return base[n];
    }
    
    seq_t ring[8];
    for (int i = 0; i < 8; i++) {
        ring[i] = base[i + 1];
    }
    
    for (idx_t i = 9; i <= n; i++) {
        seq_t next = ring[6] + ring[3];
        for (int j = 0; j < 7; j++) {
            ring[j] = ring[j + 1];
        }
        ring[7] = next;
    }
    
    return ring[7];
}
