/*
 * A057348  Days in months in the Islamic calendar starting from Muharram, 1
 *          AH. The twelfth month has 30 days in a leap year.
 * https://oeis.org/A057348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29 + (m mod 2) + floor(m/12) * (floor((11*(y+1)+3)/30) -
 *            floor((11*y+3)/30)) where y = floor((n-1)/12) + 1 /* Islamic
 *            year number and m = ((n-1) mod 12) + 1 /* Islamic month number
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1 */
    idx_t nm1 = n - 1;
    idx_t y   = nm1 / 12 + 1;          /* year number */
    idx_t m   = nm1 % 12 + 1;          /* month number */

    /* Base length: 29 days plus 1 day for odd-numbered months */
    seq_t val = 29 + (seq_t)(m & 1);

    /* Leap‑year adjustment for the 12th month */
    if (m == 12) {
        idx_t rem = (11 * y + 3) % 30;
        /* leap month if remainder >= 19 */
        if (rem >= 19) {
            val += 1;
        }
    }
    return val;
}
