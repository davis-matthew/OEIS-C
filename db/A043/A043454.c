/*
 * A043454  Numbers having two 0's in base 9.
 * https://oeis.org/A043454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑9 representation
 *            contains exactly two zeros
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, so n==0 is outside the sequence */
    idx_t found = 0;
    for (idx_t m = 1; ; ++m) {
        idx_t x = m;
        idx_t zeros = 0;
        while (x > 0) {
            if ((x % 9) == 0) {
                ++zeros;
                if (zeros > 2) break;    /* early exit if too many zeros */
            }
            x /= 9;
        }
        if (zeros == 2) {
            ++found;
            if (found == n) {
                return (seq_t)m;
            }
        }
    }
}
