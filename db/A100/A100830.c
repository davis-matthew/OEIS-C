/*
 * A100830  Smallest number with same digital root as n but distinct from n
 *          and all earlier occurrences.
 * https://oeis.org/A100830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + 9*(-1)^floor((n-1)/9)
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = (n - 1) / 9;
    if ((q & 1) != 0) {
        return (seq_t)n - 9;
    } else {
        return (seq_t)n + 9;
    }
}
