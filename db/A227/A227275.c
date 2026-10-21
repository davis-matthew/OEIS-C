/*
 * A227275  Positive solutions of the Pell equation x^2 - 97*y^2 = -1.
 *          Solutions y = 569*a(n).
 * https://oeis.org/A227275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*62809633*a(n-1) - a(n-2); a(0) = 1, a(1) = 125619265
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)125619265;
    }

    seq_t prevprev = (seq_t)1;
    seq_t prev     = (seq_t)125619265;
    seq_t cur;
    const seq_t C = (seq_t)125619266; /* 2 * 62809633 */

    for (idx_t i = 2; i <= n; ++i) {
        cur = C * prev - prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
