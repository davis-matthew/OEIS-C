/*
 * A138814  Divisors of 4064 (half the 4th perfect number).
 * https://oeis.org/A138814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = 2^(n-1) for 1 ≤ n ≤ 6; a(n) = 127·2^(n-7) for 7 ≤ n ≤ 12
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* invalid index, return 0 */
    if (n <= 6) {
        return ((seq_t)1 << (unsigned)(n-1));
    }
    if (n <= 12) {
        seq_t pow2 = ((seq_t)1 << (unsigned)(n-7));
        return ((seq_t)127 * pow2);
    }
    return 0;                                  /* sequence has only 12 terms */
}
