/*
 * A061100  Squares with digital root 4.
 * https://oeis.org/A061100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2m+1) = (2 + 9m)^2 for m ≥ 0 (odd indices) a(2m) = (7 +
 *            9(m–1))^2 for m ≥ 1 (even indices)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 1; return 0 safely */
        return 0;

    seq_t base;
    if (n & 1U) {                 /* n is odd: n = 2m+1, m = n/2 */
        seq_t m = n / 2;
        base = (seq_t)2 + ((seq_t)9 * m);
    } else {                      /* n is even: n = 2m, m = n/2 */
        seq_t m = n / 2 - 1;
        base = (seq_t)7 + ((seq_t)9 * m);
    }
    return base * base;
}
