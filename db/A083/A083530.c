/*
 * A083530  a(n) = 7^n mod (2*n).
 * https://oeis.org/A083530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n mod (2*n)
 * Range:     u8 n <= 144, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 nothing is defined; return 1 as harmless default */
    if (n == 0u) {
        return (seq_t)1;
    }

    seq_t mod   = (seq_t)n * (seq_t)2;   /* 2*n */
    seq_t base  = (seq_t)7;
    seq_t result = (seq_t)1;

    idx_t exp = n;                       /* exponent as idx_t for bitwise ops */
    while (exp) {
        if (exp & 1u) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1u;
    }
    return result;
}
