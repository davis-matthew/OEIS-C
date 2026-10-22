/*
 * A000601  Expansion of 1/((1-x)^2*(1-x^2)*(1-x^3)).
 * https://oeis.org/A000601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((4*n^3+42*n^2+140*n+102+21*(1+(-1)^n))/8 -
 *            6*floor((2*n+5+3*(-1)^n)/12))/18
 * Range:     u8 n <= 0, u16 n <= 21, u32 n <= 1036, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t ns2 = ns * ns;
    seq_t ns3 = ns2 * ns;
    
    int is_odd = n & 1;
    
    seq_t parity_factor = is_odd ? 0 : 2;
    seq_t numerator = 4*ns3 + 42*ns2 + 140*ns + 102 + 21*parity_factor;
    seq_t part1 = numerator / 8;
    
    seq_t floor_arg = is_odd ? (2*ns + 2) : (2*ns + 8);
    seq_t part2 = 6 * (floor_arg / 12);
    
    seq_t result = (part1 - part2) / 18;
    
    return result;
}
