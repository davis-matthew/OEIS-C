/*
 * A010200  Continued fraction for sqrt(141).
 * https://oeis.org/A010200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 11; for n >= 1, a(n) = [1,6,1,22][(n-1) mod 4] */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    idx_t r = (n - 1) & 3;          /* n-1 mod 4, safely for unsigned */
    switch (r) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)6;
        case 2: return (seq_t)1;
        case 3: return (seq_t)22;
    }
    return (seq_t)0;  /* unreachable, but keeps compiler happy */
}
