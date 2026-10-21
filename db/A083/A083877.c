/*
 * A083877  Absolute value of determinant of n X n matrix where the element
 *          a(i,j) = if i + j > n then 2*(i + j -n) - 1, else 2*(n + 1 - i -
 *          j).
 * https://oeis.org/A083877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A083877
   Recurrence:
      a(1) = 1,  a(2) = 5,  a(3) = 25
      a(n) = 5*a(n-1) - 3*a(n-2) - 9*a(n-3)   for n ≥ 4
*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)25;

    /* Maintain the last three terms */
    seq_t a_n_minus_3 = (seq_t)1;   /* a(1) */
    seq_t a_n_minus_2 = (seq_t)5;   /* a(2) */
    seq_t a_n_minus_1 = (seq_t)25;  /* a(3) */
    seq_t a_n = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        a_n = 5 * a_n_minus_1 - 3 * a_n_minus_2 - 9 * a_n_minus_3;
        /* shift the window for next iteration */
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n;
}
