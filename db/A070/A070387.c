/*
 * A070387  a(n) = 5^n mod 41.
 * https://oeis.org/A070387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n mod 41
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod  = (seq_t)41;
    const seq_t base = (seq_t)5;
    seq_t   result = (seq_t)1;
    seq_t   b      = base;
    idx_t   e      = n;

    while (e > 0) {
        if (e & 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        e >>= 1;
    }

    return result;
}
