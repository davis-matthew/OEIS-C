/*
 * A080413  Take the rightmost three binary digits of n (for n<4 padded with
 *          leading zeros) and rotate left 1 digit.
 * https://oeis.org/A080413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = keep the higher bits of n unchanged and rotate its lowest
 *            three bits left by 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* extract the lowest three bits of n */
    seq_t low = (seq_t)n & (seq_t)7;

    /* rotate them left by one position within the 3‑bit window */
    seq_t rot = ((low << 1) | (low >> 2)) & (seq_t)7;

    /* preserve the remaining higher bits of n */
    seq_t high = (seq_t)n & ~((seq_t)7);

    /* combine the unchanged higher part with the rotated low part */
    return high | rot;
}
