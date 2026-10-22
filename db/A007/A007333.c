/*
 * A007333  An upper bound on the biplanar crossing number of the complete
 *          graph on n nodes.
 * https://oeis.org/A007333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(4*k) = k*(k-1)*(k-2)*(7*k-3)/6 a(4*k+1) =
 *            k*(k-1)*(7*k^2-10*k+4)/6 a(4*k+2) = k*(k-1)*(7*k^2-3*k-1)/6
 *            a(4*k+3) = k^2*(k-1)*(7*k+4)/6 where k = floor(n/4)
 * Range:     u8 n <= 18, u16 n <= 65, u32 n <= 772, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t k = (seq_t)(n / 4);
    idx_t residue = n % 4;
    
    if (residue == 0) {
        seq_t prod = k * (k - 1) * (k - 2) / 6;
        return prod * (7*k - 3);
    } else if (residue == 1) {
        seq_t prod = k * (k - 1) / 2;
        seq_t term = 7*k*k - 10*k + 4;
        return prod * term / 3;
    } else if (residue == 2) {
        seq_t prod = k * (k - 1) / 2;
        seq_t term = 7*k*k - 3*k - 1;
        return prod * term / 3;
    } else {
        seq_t prod = k * k * (k - 1) / 2;
        return prod * (7*k + 4) / 3;
    }
}
