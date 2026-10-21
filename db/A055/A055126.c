/*
 * A055126  Base-16 complement of n (write n in base 16, then replace each
 *          digit with its base-16 negative).
 * https://oeis.org/A055126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = sum over the base‑16 digits of n of (–digit mod 16) ·
 *            16^i * * The sequence “base‑16 complement of n” is obtained by
 *            writing n in base‑16, * negating each digit modulo 16 (i.e.
 *            mapping d → (–d) & 0xF, which gives * 0→0, 1→F, 2→E, …, F→1),
 *            and interpreting the resulting digit string as * a...
 * Range:     u8 n <= 255, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    unsigned shift = 0;          /* multiplied by 16^i: shift by 4*i bits */

    while (n != 0) {
        unsigned d  = (unsigned)(n & 0xFULL);          /* current hex digit */
        unsigned nd = ((- (int)d) & 0xFULL);           /* (–digit) mod 16 */
        result |= ((seq_t)nd << shift);
        shift += 4;
        n >>= 4;
    }

    return result;
}
