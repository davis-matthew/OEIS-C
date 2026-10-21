/*
 * A212294  Sums of (zero or more) distinct twin primes.
 * https://oeis.org/A212294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 (n=1), 3 (n=2), 5 (n=3), 7 (n=4), 8 (n=5), n+4 (n>=6)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)5;
    if (n == 4)
        return (seq_t)7;
    if (n == 5)
        return (seq_t)8;
    return (seq_t)n + (seq_t)4;
}
