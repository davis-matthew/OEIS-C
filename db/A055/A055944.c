/*
 * A055944  a(n) = n + (reversal of base-2 digits of n) (written in base 10).
 * https://oeis.org/A055944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + reverse_binary(n) where reverse_binary(n) is the
 *            number obtained by reversing the binary digit string of n with
 *            no leading zeroes
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reverse the binary digits of n (using 64‑bit arithmetic). */
    idx_t m = n;
    idx_t rev = 0;
    while (m)
    {
        rev = (rev << 1) | (m & 1);
        m >>= 1;
    }
    /* Convert to seq_t before addition to respect the target type. */
    return (seq_t)n + (seq_t)rev;
}
