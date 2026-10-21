/*
 * A074068  Zigzag modulo 7.
 * https://oeis.org/A074068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n for n <= 3 a(n) = 7*floor(n/7) + 14*floor((n mod 7)/4)
 *            - (n mod 7) for n > 3 (self-inverse permutation with a(n)=n when
 *            n ≡ 0 (mod 7))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)n;

    idx_t mod7     = n % 7;                 // remainder of n mod 7
    seq_t base     = (seq_t)(7 * (n / 7));  // 7 * floor(n/7)
    seq_t adj      = (seq_t)(14 * (mod7 / 4)); // 14 * floor((n mod 7)/4)

    return base + adj - (seq_t)mod7;
}
