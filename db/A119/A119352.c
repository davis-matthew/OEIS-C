/*
 * A119352  Smallest base b > 1 such that n in base b uses no digit b-1.
 * https://oeis.org/A119352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: For each n, find the smallest base b > 1 such that
   the representation of n in base b contains no digit b-1. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;   /* base 2 works for n = 0 */
    idx_t b;
    for (b = 2; ; ++b) {
        idx_t temp = n;
        int bad = 0;
        while (temp > 0) {
            if ((temp % b) == b - 1) { bad = 1; break; }
            temp /= b;
        }
        if (!bad) return (seq_t)b;
    }
}
