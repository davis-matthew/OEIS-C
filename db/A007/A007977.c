/*
 * A007977  Modified de Bruijn sequences: max. length of binary necklace in
 *          which no n-bit word appears twice.
 * https://oeis.org/A007977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) + (n-1) + (n even ? floor(n/4) + 1 : 0)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power = ((seq_t)1) << (n - 1);
    seq_t linear = (seq_t)(n - 1);
    seq_t correction = 0;
    
    if (n % 2 == 0) {
        correction = (seq_t)(n / 4) + 1;
    }
    
    return power + linear + correction;
}
