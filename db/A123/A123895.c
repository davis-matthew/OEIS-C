/*
 * A123895  Restricted growth string for the (decimal expansion of the) number
 *          n.
 * https://oeis.org/A123895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   maps the decimal digits of n prefixed with a leading 0 to a *
 *            restricted growth string. The resulting string is interpreted as
 *            a * base‑10 number with any leading zeros removed (a(0) = 0)
 * Range:     u8 n <= 999, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* maximum number of decimal digits for a 64‑bit integer is 20,
       plus the prefixed zero and one extra for safety. */
    char tmp[32];
    int len = 0;

    /* special case n == 0 */
    if (n == 0) {
        tmp[0] = '0';
        len = 1;
    } else {
        idx_t x = n;
        while (x > 0) {
            tmp[len++] = (char)('0' + (x % 10));
            x /= 10;
        }
        /* reverse to obtain most‑significant digit first */
        for (int i = 0; i < len / 2; ++i) {
            char c = tmp[i];
            tmp[i] = tmp[len - 1 - i];
            tmp[len - 1 - i] = c;
        }
    }

    /* Build the prefixed zero string */
    char digits[32];
    int pos = 0;
    digits[pos++] = '0';             /* leading zero */
    for (int i = 0; i < len; ++i) {
        digits[pos++] = tmp[i];      /* the digits of n */
    }

    /* Map each distinct decimal digit to the smallest
       non‑negative integer not yet used (restricted growth) */
    int digit_map[10];
    for (int i = 0; i < 10; ++i) digit_map[i] = -1;
    int next_code = 0;

    int mapped[32];
    int rlen = 0;
    for (int i = 0; i < pos; ++i) {
        int d = digits[i] - '0';
        if (digit_map[d] == -1) {
            digit_map[d] = next_code++;
        }
        mapped[rlen++] = digit_map[d];
    }

    /* Skip leading zeros */
    int start = 0;
    while (start < rlen && mapped[start] == 0) ++start;
    if (start == rlen) return 0;   /* all zeros => value 0 */

    /* Convert the remaining digits to a seq_t value */
    seq_t result = 0;
    for (int i = start; i < rlen; ++i) {
        result = result * 10 + mapped[i];
    }

    return result;
}
