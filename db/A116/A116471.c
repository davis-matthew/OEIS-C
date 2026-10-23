/*
 * A116471  Values 2*(n -+ 1)^2 sorted.
 * https://oeis.org/A116471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; for n > 1, a(n) = 2 * ((n + 1) / 2)^2
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    seq_t k = ((seq_t)n + 1) / 2;
    return 2 * k * k;
}
