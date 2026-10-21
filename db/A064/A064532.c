/*
 * A064532  Total number of holes in decimal expansion of the number n,
 *          assuming 4 has no hole.
 * https://oeis.org/A064532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 7999, u16 n <= 7999, u32 n <= 7999, u64 n <= 7999,
 *            u128 n <= 7999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the total number of closed loops ("holes") in the decimal
   representation of n, treating the digit 4 as having no holes.
   Digits: 0,6,9 each contribute one hole; 8 contributes two holes; all
   other digits contribute zero.  a(0) = 1 because the single digit
   "0" contains one hole. */
seq_t a(idx_t n) {
    /* Special case: 0 is represented as a single digit 0. */
    if (n == 0) return 1;

    seq_t holes = 0;
    idx_t x = n;
    while (x != 0) {
        int d = x % 10;
        switch (d) {
            case 0: holes += 1; break;
            case 6: holes += 1; break;
            case 9: holes += 1; break;
            case 8: holes += 2; break;
            default: /* 1,2,3,4,5,7 have no holes */ break;
        }
        x /= 10;
    }
    return holes;
}
