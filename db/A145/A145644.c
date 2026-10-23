/*
 * A145644  Cubefree part of 10^n.
 * https://oeis.org/A145644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 10^(n mod 3)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence repeats 1, 10, 100 with period 3. */
    switch (n % 3ULL) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)10;
        case 2: return (seq_t)100;
        default: return (seq_t)0; /* never reached */
    }
}
