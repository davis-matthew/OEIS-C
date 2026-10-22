/*
 * A004510  Tersum n + 21.
 * https://oeis.org/A004510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 21) where tersum adds n and 21 digit-by-digit
 *            in base 3, mod 3
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t base_value = 21;
    seq_t result = 0;
    seq_t power_of_3 = 1;
    
    while (n_val > 0 || base_value > 0) {
        seq_t sum_digit = (n_val % 3 + base_value % 3) % 3;
        result += sum_digit * power_of_3;
        
        n_val /= 3;
        base_value /= 3;
        power_of_3 *= 3;
    }
    
    return result;
}
