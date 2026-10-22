/*
 * A007913  Squarefree part of n: a(n) is the smallest positive number m such
 *          that n/m is a square.
 * https://oeis.org/A007913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, nice
 * Formula:   a(n) = n with all perfect square factors removed
 * Range:     u8 n <= 256, u16 n <= 9302, u32 n <= 9302, u64 n <= 9302,
 *            u128 n <= 9302, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t temp = (seq_t)n;
    seq_t i = 2;
    
    while (i <= temp / i) {
        seq_t i_sq = i * i;
        while (temp % i_sq == 0) {
            temp = temp / i_sq;
        }
        if (temp % i == 0) {
            result = result * i;
            temp = temp / i;
        }
        i++;
    }
    
    if (temp > 1) {
        result = result * temp;
    }
    
    return result;
}
