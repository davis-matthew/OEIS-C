/*
 * A003878  a(n) = n^4 + (9/2)*n^3 + n^2 - (9/2)*n + 1.
 * https://oeis.org/A003878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + (9/2)*n^3 + n^2 - (9/2)*n + 1
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n4 = n2 * n2;
    
    // a(n) = n^4 + n^2 + 1 + (9/2)*n*(n-1)*(n+1)
    // n*(n-1)*(n+1) is always even, so divide by 2 early to avoid overflow
    seq_t middle;
    if (n % 2 == 0) {
        middle = 9 * (n_seq / 2) * (n_seq - 1) * (n_seq + 1);
    } else {
        middle = 9 * n_seq * ((n_seq - 1) / 2) * (n_seq + 1);
    }
    
    return n4 + n2 + 1 + middle;
}
