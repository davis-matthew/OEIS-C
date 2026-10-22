/*
 * A000855  Final two digits of 2^n.
 * https://oeis.org/A000855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^n mod 100
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    uint64_t result = 1;
    uint64_t base = 2;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % 100;
        }
        base = (base * base) % 100;
        exp >>= 1;
    }
    
    return (seq_t)result;
}
