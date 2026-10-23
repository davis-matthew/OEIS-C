/*
 * A130146  n appears k times, where k = last digit of n.
 * https://oeis.org/A130146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the value at position n in the sequence where each
 *            integer k appears last_digit(k) times. (0 last digits contribute
 *            0 copies.)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;   /* current integer value */
    idx_t idx = 1;   /* index of the first term contributed by current val */

    while (1) {
        unsigned ld = (unsigned)(val % 10);   /* last digit of val */
        if (ld != 0) {                       /* val appears ld times */
            if (n <= idx + ld - 1)           /* target lies in this block */
                return val;
            idx += ld;                       /* skip over block */
        }
        val++;                               /* next integer value */
    }
}
