/*
 * A120634  Decimal equivalent of A066335.
 * https://oeis.org/A120634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS entry: A120634 – Decimal equivalent of A066335.
 * Formula (direct implementation):
 *   a(0) = 0
 *   for n >= 1:
 *     q = (n-1) / 4          // integer division
 *     r = (n-1) % 4
 *     a(n) = 7 + 4*q - r     // yields the decreasing blocks 7,6,5,4, 11,10,9,8, …
 *   This satisfies the recursive definition:
 *     a(n) = a(n-1) + a(n-4) - a(n-5)   (for n >= 5).
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute q = (n-1)/4 and r = (n-1)%4 */
    idx_t t = n - 1U;          /* t fits in idx_t */
    idx_t q = t / 4U;
    idx_t r = t % 4U;

    /* Compute result in seq_t, converting as needed */
    seq_t result = (seq_t)7
                 + ((seq_t)4 * (seq_t)q)
                 - (seq_t)r;

    return result;
}
