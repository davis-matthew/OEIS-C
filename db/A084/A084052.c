/*
 * A084052  2*n digit-reversed mod 2.
 * https://oeis.org/A084052
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = rev(2*n) mod 2
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 5. For smaller n the value is
       unspecified, so we return 0. */
    if (n < 5)
        return (seq_t)0;

    /* Compute 2*n using the native integer type to avoid overflow
       in the (potentially small) seq_t type. */
    idx_t m = n * (idx_t)2;

    /* Extract the most significant decimal digit of 2*n.  This is
       the first digit of the reversed number.  Then take its parity
       (odd → 1, even → 0). */
    while (m >= 10)
        m /= 10;

    return (seq_t)(m % 2);
}
