/*
 * A102669  Number of digits >= 2 in decimal representation of n.
 * https://oeis.org/A102669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of base‑10 digits of n that are >= 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count digits >= 2 */
    seq_t count = 0;
    idx_t m = n;

    while (m > 0) {
        /* Extract lowest decimal digit */
        uint64_t d = m % 10UL;
        if (d >= 2) {
            count++;
        }
        m /= 10UL;
    }

    /* a(0) = 0 is already handled by the loop not executing */
    return count;
}
