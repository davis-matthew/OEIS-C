/*
 * A154240  a(n) = ( (8 + sqrt(6))^n - (8 - sqrt(6))^n )/(2*sqrt(6)).
 * https://oeis.org/A154240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 58*a(n-2); a(1)=1; a(2)=16
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)16;  /* a(n-1) */
    seq_t curr  = prev1;      /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)16 * prev1 - (seq_t)58 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
