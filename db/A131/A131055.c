/*
 * A131055  1 followed by repeats of 2*k.
 * https://oeis.org/A131055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = ((-1)^n + 2*n - 1) / 2 which
 *            simplifies to: a(n) = n if n is even a(n) = n - 1 if n is odd
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    return (n & 1) ? (seq_t)(n - 1) : (seq_t)n;
}
