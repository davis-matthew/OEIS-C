/*
 * A006047  Number of entries in n-th row of Pascal's triangle not divisible
 *          by 3.
 * https://oeis.org/A006047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^s * 2^r, where n in base 3 has r ones and s twos
 * Range:     u8 n <= 403, u16 n <= 9506, u32 n <= 9506, u64 n <= 9506,
 *            u128 n <= 9506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count_ones = 0, count_twos = 0;
    idx_t temp = n;
    
    // Convert to base 3 and count 1's and 2's
    while (temp > 0) {
        int digit = temp % 3;
        if (digit == 1) {
            count_ones++;
        } else if (digit == 2) {
            count_twos++;
        }
        temp /= 3;
    }
    
    // Compute 3^count_twos * 2^count_ones
    seq_t result = (seq_t)1;
    
    // Multiply by 2 count_ones times
    for (idx_t i = 0; i < count_ones; i++) {
        result = result * (seq_t)2;
    }
    
    // Multiply by 3 count_twos times
    for (idx_t i = 0; i < count_twos; i++) {
        result = result * (seq_t)3;
    }
    
    return result;
}
