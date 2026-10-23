/*
 * A161149  a(n) = (2*n)!*(2*n+1)!/n! = n!*A000909(n).
 * https://oeis.org/A161149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)! * (2*n+1)! / n!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n>0: a(n) = a(n-1) * 4*n * (4*n*n - 1) */

seq_t a(idx_t n)
{
    seq_t result = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t t1 = (seq_t)4 * (seq_t)i;                     /* 4*n       */
        seq_t t2 = (seq_t)4 * (seq_t)i * (seq_t)i - 1;      /* 4*n*n - 1 */
        result *= t1 * t2;                                 /* Multiply by 4*n*(4*n*n-1) */
    }

    return result;
}
