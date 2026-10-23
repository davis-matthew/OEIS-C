/*
 * A163241  Simple self-inverse permutation: Write n in base 4, then replace
 *          each digit '2' with '3' and vice versa, then convert back to
 *          decimal.
 * https://oeis.org/A163241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = convert n to base-4, swap digits 2↔3, and convert back to
 *            decimal
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t t = n;
    seq_t res = 0;
    seq_t pw = 1;
    while (t != 0) {
        unsigned d = t & 3U;  /* t % 4 */
        t >>= 2;              /* t /= 4 */
        if (d == 2) d = 3;
        else if (d == 3) d = 2;
        res += (seq_t)d * pw;
        pw *= 4;
    }
    return res;
}
