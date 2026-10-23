/*
 * A047216  Numbers that are congruent to {1, 2} mod 5.
 * https://oeis.org/A047216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n - 3*(-1)^n - 9)/4 Equivalent piece‑wise: a(n) = 5*(
 *            (n+1)/2 ) - 4 if n is odd a(n) = 5*( n/2 ) - 3 if n is even
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* As offset is 1, behaviour for n==0 is irrelevant; return 0 for safety. */
    if (n == 0) return 0;

    if ((n & 1) == 0) {            /* n even : a(n) = 5*(n/2) - 3 */
        seq_t k = (seq_t)(n / 2);
        return ((seq_t)5 * k) - 3;
    } else {                       /* n odd  : a(n) = 5*((n+1)/2) - 4 */
        seq_t k = (seq_t)((n + 1) / 2);
        return ((seq_t)5 * k) - 4;
    }
}
