/*
 * A174630  A weight function for the case N = 24 and k = 6 in Butler-Graham
 *          shuffling.
 * https://oeis.org/A174630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   For a given n, * let m = n / 6 (integer division) * let r = n %
 *            6 * let s = r + floor(r/2) // r = 0,1,4,5 → s = r + r/2 * if r
 *            == 2 or r == 3: s += 1 // adjust for r = 2,3 * if r == 2 or r ==
 *            3: * a = s + ((m & 1) - ((m >> 1) & 1)) // gives +1, 0, -1, or
 *            +0 depending on m % 4 * else: * a = s + ce...
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n / 6;
    idx_t r = n % 6;

    seq_t s = (seq_t)r + (seq_t)(r / 2);

    if (r == 2 || r == 3) {
        s += (seq_t)1;
        /* d = ((m & 1) - ((m >> 1) & 1)) which yields 0, +1, -1, or 0 */
        long long d = ((long long)(m & 1)) - ((long long)((m >> 1) & 1));
        return (seq_t)((long long)s + d);
    } else {
        return s + (seq_t)((m + 1) / 2);
    }
}
