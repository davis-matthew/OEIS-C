/*
 * A070386  a(n) = 5^n mod 39.
 * https://oeis.org/A070386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 39 (period 4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 4 (periodicity of 5^n modulo 39). */
    idx_t r = n & 3;                 /* n % 4 for unsigned arithmetic */
    switch (r) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)5;
    case 2: return (seq_t)25;
    case 3: return (seq_t)8;
    }
    /* Unreachable but required for all control paths */
    return (seq_t)0;
}
