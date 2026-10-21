/*
 * A092464  Numbers congruent to 4 or 9 mod 13.
 * https://oeis.org/A092464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*⌊(n-1)/2⌋ + (4 if n is odd, 9 if n is even)
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; n==0 is undefined, return 0 gracefully */
        return 0;
    idx_t q = (n - 1) / 2;                    /* floor((n-1)/2) */
    seq_t base = (seq_t)q * (seq_t)13;
    return base + (n & 1 ? (seq_t)4 : (seq_t)9);
}
