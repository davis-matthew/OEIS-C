/*
 * A216096  a(n) = 3^n mod 1000.
 * https://oeis.org/A216096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 1000
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long result = 1ULL;
    unsigned long long base   = 3ULL;
    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL) {
            result = (result * base) % 1000ULL;
        }
        base = (base * base) % 1000ULL;
        exp >>= 1;
    }
    return (seq_t)result;
}
