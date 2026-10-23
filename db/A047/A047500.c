/*
 * A047500  Numbers that are congruent to {0, 3, 4, 5, 7} mod 8.
 * https://oeis.org/A047500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/5) + [0,3,4,5,7][(n-1)%5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     // behavior for n < 1 is irrelevant
    idx_t k = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    static const seq_t shift[5] = { (seq_t)0, (seq_t)3, (seq_t)4, (seq_t)5, (seq_t)7 };
    return (seq_t)k * (seq_t)8 + shift[r];
}
