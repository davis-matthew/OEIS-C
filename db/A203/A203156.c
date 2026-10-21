/*
 * A203156  (n-1)-st elementary symmetric function of {4,9,16,25,...,
 *          (n+1)^2}.
 * https://oeis.org/A203156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)!^2 * sum_{i=2}^{n+1} 1/i^2 Recurrence: a(1) = 1
 *            a(n) = (n+1)^2 * a(n-1) + (n!)^2 for n >= 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t fact = (seq_t)1;        /* fact = (i-1)! while loop iterates */
    seq_t a_val = (seq_t)1;        /* current a(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;                    /* fact = i!  */
        seq_t term = fact * fact;           /* (i!)^2   */
        seq_t factor = (seq_t)(i + 1);      /* (i+1)     */
        factor *= factor;                  /* (i+1)^2   */
        a_val = a_val * factor + term;     /* recurrence */
    }
    return a_val;
}
