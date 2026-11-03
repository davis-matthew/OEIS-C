/*
 * A006257  Josephus problem: a(2*n) = 2*a(n)-1, a(2*n+1) = 2*a(n)+1.
 * https://oeis.org/A006257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2(n - msb(n)) + 1
 * Range:     u8 n <= 382, u16 n <= 98302, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 95 terms through n = 9098 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    if (n_seq == 0) return 0;
    
    seq_t msb = 1;
    while ((msb << 1) > msb && (msb << 1) <= n_seq) {
        msb <<= 1;
    }
    
    return (((n_seq - msb) << 1) + 1);
}
