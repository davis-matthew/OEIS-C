/*
 * A007379  Earliest sequence with a(a(n)) = 4n.
 * https://oeis.org/A007379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(8n) = 4*a(2n), a(8n+1) = 8n+2, a(8n+2) = 32n+4, a(8n+3) =
 *            8n+5, a(8n+4) = 4*a(2n+1), a(8n+5) = 32n+12, a(8n+6) = 8n+7,
 *            a(8n+7) = 32n+24
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    idx_t q = n / 8;
    idx_t r = n % 8;
    seq_t sq = (seq_t)q;
    
    switch (r) {
        case 0:
            return 4 * a(2 * q);
        case 1:
            return 8 * sq + 2;
        case 2:
            return 32 * sq + 4;
        case 3:
            return 8 * sq + 5;
        case 4:
            return 4 * a(2 * q + 1);
        case 5:
            return 32 * sq + 12;
        case 6:
            return 8 * sq + 7;
        case 7:
            return 32 * sq + 24;
    }
    return 0;
}
