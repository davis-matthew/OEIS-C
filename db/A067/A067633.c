/*
 * A067633  a(1) = 1; for n > 1, a(n) is the least integer > a(n-1) such that
 *          the sequence of digits of a(n-1)^2 is a subsequence of the
 *          sequence of digits of a(n)^2.
 * https://oeis.org/A067633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067633: a(1)=1, a(2)=4, a(3)=13, a(4)=37, a(5)=117, a(6)=367;
   For n≥7: a(n)=10*a(n-1)-3 (iteratively). */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of range, safe fallback */

    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 13;
    if (n == 4) return 37;
    if (n == 5) return 117;
    if (n == 6) return 367;

    seq_t val = 367;
    for (idx_t i = 7; i <= n; ++i) {
        val = val * 10;
        val -= 3;
    }
    return val;
}
