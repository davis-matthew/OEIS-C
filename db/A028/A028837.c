/*
 * A028837  Iterated sum of digits of n is a square.
 * https://oeis.org/A028837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*n if n % 3 == 0 else 3*n - 2 (OEIS A028837) Initial
 *            terms: a(1)=1, a(2)=4, a(3)=9
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 3 == 0)
        return (seq_t)3 * n;
    else
        return (seq_t)3 * n - (seq_t)2;
}
