/*
 * A070411  a(n) = 7^n mod 26.
 * https://oeis.org/A070411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod 26
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 7 % 26;            /* 7 is already <26 */
    while (n > 0) {
        if (n & 1ull) {
            result = (result * base) % 26;
        }
        base = (base * base) % 26;
        n >>= 1;
    }
    return result;
}
