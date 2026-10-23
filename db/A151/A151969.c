/*
 * A151969  a(n) = smallest integer >= n which has only prime factors 2 and 5.
 * https://oeis.org/A151969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer >= n whose prime factors are only 2 and
 *            5
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)n;
    for (;; ++candidate)
    {
        seq_t t = candidate;
        while (t % 2 == 0) t /= 2;
        while (t % 5 == 0) t /= 5;
        if (t == 1) return candidate;
    }
}
