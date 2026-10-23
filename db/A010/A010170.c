/*
 * A010170  Continued fraction for sqrt(99).
 * https://oeis.org/A010170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=9; for n>0, a(n)=18^(n-1) mod 19 (period 2 after the
 *            initial 9)
 * Range:     u8 n <= 9804, u16 n <= 9804, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)9;
    }
    /* 18^k mod 19 is 1 if k even, 18 if k odd because 18^2 ≡ 1 (mod 19) */
    if (((n - 1) & 1) == 0) {
        return (seq_t)1;
    } else {
        return (seq_t)18;
    }
}
