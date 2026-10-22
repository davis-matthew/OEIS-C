/*
 * A000007  The characteristic function of {0}: a(n) = 0^n.
 * https://oeis.org/A000007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nonn, mult, cons, easy
 * Formula:   a(n) = 0^n, i.e. a(0) = 1 and a(n) = 0 for n >= 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    if (n == 0)
        r = 1;
    return r;
}
