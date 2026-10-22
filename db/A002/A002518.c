/*
 * A002518  Earliest sequence with a(a(n))=5n.
 * https://oeis.org/A002518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(5n) = 5*a(n), a(5n+1) = 5n+2, a(5n+2) = 25n+5, a(5n+3) = 5n+4,
 *            a(5n+4) = 25n+15
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    idx_t q = n / 5;
    idx_t r = n % 5;
    
    switch (r) {
        case 0: return (seq_t)5 * a(q);
        case 1: return (seq_t)5 * (seq_t)q + 2;
        case 2: return (seq_t)25 * (seq_t)q + 5;
        case 3: return (seq_t)5 * (seq_t)q + 4;
        default: return (seq_t)25 * (seq_t)q + 15;
    }
}
