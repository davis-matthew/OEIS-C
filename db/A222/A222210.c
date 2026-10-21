/*
 * A222210  In the number n, replace all (decimal) digits '0' with '1' and
 *          vice versa.
 * https://oeis.org/A222210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace all decimal digits 0 with 1 and 1 with 0 in n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* special case for n = 0 (decimal "0" becomes "1") */
    if (n == 0) return (seq_t)1;

    seq_t result = 0;
    seq_t mult   = 1;    /* current place value: 1, 10, 100, ... */

    while (n > 0) {
        int d = n % 10;        /* extract least‑significant decimal digit */
        n /= 10;               /* remove extracted digit */

        /* digit replacement rules */
        if (d == 0) d = 1;
        else if (d == 1) d = 0;

        result += (seq_t)d * mult;
        mult   *= 10;
    }

    return result;
}
