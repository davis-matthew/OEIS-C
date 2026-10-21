/*
 * A080414  Take the rightmost three binary digits of n (for n<4 padded with
 *          leading zeros) and rotate right 1 digit.
 * https://oeis.org/A080414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n & ~7) + r[n & 7], with r = {0,4,1,5,2,6,3,7}
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Takes the right‑most three binary bits of n, rotates them right by one, and keeps
   the remaining higher bits unchanged. */
seq_t a(idx_t n)
{
    static const seq_t r[8] = {0,4,1,5,2,6,3,7};
    return ((seq_t)(n & ~((seq_t)7))) + r[n & 7];
}
