/*
 * A000392  Stirling numbers of second kind S(n,3).
 * https://oeis.org/A000392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = 3*a(n-1) + 2^(n-2) - 1 for n >= 4, a(0) = a(1) = a(2) =
 *            0, a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;

    seq_t s = 1;   /* a(3) */
    seq_t p = 2;   /* 2^(k-3), becomes 2^(k-2) inside loop */
    for (idx_t k = 4; k <= n; k++) {
        p *= 2;
        s = s * 3 + (p - 1);
    }
    return s;
}
