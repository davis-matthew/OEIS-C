/*
 * A190593  Maximal digit in base-4 expansion of n.
 * https://oeis.org/A190593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = maximum digit in base-4 representation of n
 * Range:     u8 n <= 595, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t maxdigit = 0;
    while (x > 0) {
        seq_t rem = x % (seq_t)4;
        if (rem > maxdigit)
            maxdigit = rem;
        x /= (seq_t)4;
    }
    return maxdigit;
}
