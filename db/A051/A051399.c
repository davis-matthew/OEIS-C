/*
 * A051399  a(n) = (n-1)!*a(n-1)+1.
 * https://oeis.org/A051399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * a(n-1) + 1 (with a(0) = 0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t a_val = (seq_t)0;      /* a(0) */
    seq_t fact  = (seq_t)1;      /* 0! */

    for (idx_t i = 1; i <= n; ++i) {
        a_val = fact * a_val + (seq_t)1;
        fact  = fact * (seq_t)i; /* fact becomes i! for next iteration */
    }

    return a_val;
}
