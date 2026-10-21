/*
 * A087446  Numbers that are congruent to {1, 6} mod 15.
 * https://oeis.org/A087446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n-1)/4 + (5*(-1)^n)/4
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence consists of the positive integers congruent to 1 or 6 modulo 15. */
    /* For odd n: a(n) = 15*((n+1)/2) - 14. */
    /* For even n: a(n) = 15*(n/2) - 9. */
    seq_t nn = (seq_t)n;
    seq_t k  = nn >> 1;              /* floor(n/2) in seq_t arithmetic */
    if (nn & 1) {                    /* n is odd */
        /* k_odd = (n+1)/2 = floor(n/2) + 1 */
        return (seq_t)15 * (k + 1) - (seq_t)14;
    } else {                         /* n is even */
        return (seq_t)15 * k - (seq_t)9;
    }
}
