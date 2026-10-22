/*
 * A004494  Tersum n + 5.
 * https://oeis.org/A004494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = tersum(n, 5) where tersum adds numbers in base 3 with no
 *            carries (mod 3 per digit)
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t result = 0;
    seq_t power = 1;
    seq_t constant = 5;
    
    while (n_val > 0 || constant > 0) {
        seq_t digit_n = n_val % 3;
        seq_t digit_const = constant % 3;
        seq_t digit_sum = (digit_n + digit_const) % 3;
        
        result += digit_sum * power;
        
        n_val /= 3;
        constant /= 3;
        power *= 3;
    }
    
    return result;
}
