/*
 * A043495  Numbers having three 1's in base 10.
 * https://oeis.org/A043495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer that contains exactly three '1'
 *            digits in its decimal representation
 * Range:     u8 n <= 1, u16 n <= 698, u32 n <= 9811, u64 n <= 9811,
 *            u128 n <= 9811, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int count_ones(seq_t v)
{
    int c = 0;
    while (v) {
        if ((v % 10) == 1) {
            if (++c > 3)
                break;
        }
        v /= 10;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset 0 */

    idx_t found = 0;
    seq_t val = 111;                     /* first term */

    while (found < n) {
        if (count_ones(val) == 3) {
            ++found;
            if (found == n)
                return val;
        }
        val += 1;
    }

    return 0;                            /* should never be reached */
}
