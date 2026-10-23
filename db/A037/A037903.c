/*
 * A037903  a(n) = (greatest base 9 digit of n) - (least base 9 digit of n).
 * https://oeis.org/A037903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (greatest base 9 digit of n) – (least base 9 digit of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t max_d = 0;
    seq_t min_d = 9;          /* larger than any base‑9 digit */
    while (n > 0) {
        idx_t d = n % 9;      /* digit in base 9, guaranteed < 9 */
        if ((seq_t)d > max_d) max_d = (seq_t)d;
        if ((seq_t)d < min_d) min_d = (seq_t)d;
        n /= 9;
    }
    return max_d - min_d;
}
