/*
 * A104344  a(n) = Sum_{k=1..n} k!^2.
 * https://oeis.org/A104344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (k!^2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A104344 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact = fact * (seq_t)i;       /* compute i! */
        seq_t term = fact * fact;     /* (i!)^2 */
        sum += term;                  /* accumulate */
    }
    return sum;
}
