/*
 * A043483  Numbers having three 7's in base 9.
 * https://oeis.org/A043483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers that have exactly three digits equal to 7 in base 9
 *            (OEIS A043483)
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t candidate = 0;
    idx_t found = 0;

    /* iterate over natural numbers until we find the nth matching one */
    while (found < n) {
        candidate++;

        /* count how many 7's appear when candidate is written in base 9 */
        idx_t temp = candidate;
        int cnt7 = 0;
        while (temp > 0) {
            if (temp % 9 == 7)
                ++cnt7;
            temp /= 9;
            if (cnt7 > 3)        /* early exit – more than three 7's */
                break;
        }

        if (cnt7 == 3) {
            ++found;
        }
    }

    return (seq_t)candidate;
}
