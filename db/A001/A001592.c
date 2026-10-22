/*
 * A001592  Hexanacci numbers: a(n+1) = a(n)+...+a(n-5) with a(0)=...=a(4)=0,
 *          a(5)=1.
 * https://oeis.org/A001592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6), n >=
 *            6 Initial values: a(0) = a(1) = a(2) = a(3) = a(4) = 0, a(5) = 1
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 5) return 0;
    if (n == 5) return 1;
    
    seq_t buf[6] = {0, 0, 0, 0, 0, 1};
    
    for (idx_t i = 6; i <= n; i++) {
        seq_t next = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }
    
    return buf[5];
}
