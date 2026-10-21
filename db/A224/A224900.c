/*
 * A224900  a(n) = n!*((n+1)!)^2.
 * https://oeis.org/A224900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * ((n+1)!)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = n * (n+1)^2 * a(n-1)  for n >= 1 */

seq_t a(idx_t n)
{
    seq_t result = 1;               /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii   = (seq_t)i;                      /* i          */
        seq_t np1  = (seq_t)(i + 1);                /* n+1        */
        seq_t factor = ii * (np1 * np1);            /* i*(n+1)^2  */
        result = result * factor;                  /* a(i)       */
    }
    return result;
}
