/*
 * A007981  Number of nonsplit type 2 metacyclic 2-groups of order 2^n.
 * https://oeis.org/A007981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: -x^8*(x^3-x-1) / ((x-1)^4*(x+1)^2*(x^2+x+1)) Recurrence:
 *            a(n) = a(n-1) + 2*a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) + 2*a(n-6)
 *            + a(n-7) - a(n-8) with adjustments for n = 8, 9, 11 (from
 *            coefficients of numerator polynomial)
 * Range:     u8 n <= 28, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 7) return 0;
    
    seq_t buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    for (idx_t k = 8; k <= n; k++) {
        seq_t new_val;
        
        if (k == 11) {
            seq_t pos = buf[7] + 2*buf[6] + 2*buf[2] + buf[1];
            seq_t neg = buf[5] + 2*buf[4] + buf[3] + buf[0] + 1;
            new_val = pos - neg;
        } else {
            seq_t adj = (k == 8 || k == 9) ? 1 : 0;
            new_val = buf[7] + 2*buf[6] - buf[5] - 2*buf[4] - buf[3] + 2*buf[2] + buf[1] - buf[0] + adj;
        }
        
        for (int i = 0; i < 7; i++) buf[i] = buf[i+1];
        buf[7] = new_val;
    }
    
    return buf[7];
}
