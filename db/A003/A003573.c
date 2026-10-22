/*
 * A003573  Order of 4 mod 4n+1.
 * https://oeis.org/A003573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 4 modulo (4n+1)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)4 * n + 1;
    
    if (m == 1)
        return 1;
    
    seq_t power = (seq_t)4 % m;
    seq_t k = 1;
    
    while (power != 1) {
        power = (power * (seq_t)4) % m;
        k++;
    }
    
    return k;
}
