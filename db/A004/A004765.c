/*
 * A004765  Numbers whose binary expansion does not begin 111.
 * https://oeis.org/A004765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5, a(7)=6 For n >=
 *            5, a(2n-2) = 2*a(n), a(2n-1) = 2*a(n)+1
 * Range:     u8 n <= 193, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 7) return n - 1;
    
    // Trace path from n down to base case (n <= 7)
    int path_len = 0;
    idx_t current = n;
    uint8_t parity[66];
    
    while (current > 7) {
        parity[path_len++] = current & 1;
        current = (current & 1) ? (current + 1) / 2 : (current + 2) / 2;
    }
    
    // Start with base case value
    seq_t result = current - 1;
    
    // Recompute going back up: a(2m-2)=2*a(m), a(2m-1)=2*a(m)+1
    for (int i = path_len - 1; i >= 0; i--) {
        result = 2 * result + parity[i];
    }
    
    return result;
}
