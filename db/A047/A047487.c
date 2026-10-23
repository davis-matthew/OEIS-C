/*
 * A047487  Numbers that are congruent to {2, 3, 5, 7} mod 8.
 * https://oeis.org/A047487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {2,3,5,7}[(n-1)%4] for n >= 1
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 4;            /* number of complete groups of 8 */
    int    r = (int)((n - 1) % 4);    /* position in the group */

    seq_t offset;
    switch (r) {
        case 0: offset = 2; break;
        case 1: offset = 3; break;
        case 2: offset = 5; break;
        case 3: offset = 7; break;
        default: offset = 0;  /* never reached */
    }

    return (seq_t)k * 8 + offset;
}
