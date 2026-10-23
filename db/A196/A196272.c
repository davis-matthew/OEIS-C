/*
 * A196272  Number of occurrences of '11' in base-4 expansion of n.
 * https://oeis.org/A196272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the number of overlapping occurrences of "11" in the
 *            base‑4 * representation of n. The implementation simply scans
 *            the digits of n in base‑4 * from least significant to most
 *            significant, counting successive 1‑digits
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t x = n;
    int prev = -1;            /* previous digit, -1 means none yet */
    while (x > 0) {
        int digit = x % 4;    /* current least‑significant digit in base 4 */
        if (digit == 1 && prev == 1)
            count++;
        prev = digit;
        x = x / 4;
    }
    return count;
}
