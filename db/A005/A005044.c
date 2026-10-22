/*
 * A005044  Alcuin's sequence: expansion of x^3/((1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A005044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = round(n^2/12) - floor(n/4)*floor((n+2)/4)
 * Range:     u8 n <= 108, u16 n <= 1770, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t n_sq = n * n;
    uint64_t rounded = (n_sq + 6) / 12;
    
    uint64_t quarter_n = n / 4;
    uint64_t quarter_n_plus_2 = (n + 2) / 4;
    uint64_t correction = quarter_n * quarter_n_plus_2;
    
    return (seq_t)(rounded - correction);
}
