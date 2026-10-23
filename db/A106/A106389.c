/*
 * A106389  Numbers j such that 6j^2 + 6j + 1 = 13k.
 * https://oeis.org/A106389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-15 + 7*(-1)^n + 26*n) / 4
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=11; a(n)=a(n-2)+13 for n≥3 */
seq_t a(idx_t n)
{
    /* n is ≥1.  For even n: a = (13*n - 4)/2; for odd n: a = (13*n - 11)/2. */
    if ((n & 1ULL) == 0) {
        /* even */
        return ((seq_t)13 * (seq_t)n - 4) / 2;
    } else {
        /* odd */
        return ((seq_t)13 * (seq_t)n - 11) / 2;
    }
}
