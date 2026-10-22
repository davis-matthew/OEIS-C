/*
 * A007503  Number of subgroups of dihedral group: sigma(n) + d(n).
 * https://oeis.org/A007503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d+1) = sigma(n) + tau(n)
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            result += (seq_t)d + 1;
            if (d * d != n) {
                result += (seq_t)(n / d) + 1;
            }
        }
    }
    
    return result;
}
