/*
 * A159514  Numerator of Hermite(n, 2/15).
 * https://oeis.org/A159514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4,
   a(n)=4*a(n-1) - 450*(n-1)*a(n-2)  for n>=2  */
seq_t a(idx_t n)
{
    if (n == 0) { return (seq_t)1; }
    if (n == 1) { return (seq_t)4; }

    seq_t a0 = (seq_t)1;       /* a(n-2)  */
    seq_t a1 = (seq_t)4;       /* a(n-1)  */
    seq_t an;                 /* a(n)   */

    for (idx_t i = 2; i <= n; ++i) {
        /* factor = 450*(i-1) as seq_t to avoid overflow of intermediate
           64‑bit multiplication when seq_t is larger. */
        seq_t factor = (seq_t)450 * (seq_t)(i - 1);
        an = a1 * (seq_t)4 - a0 * factor;
        a0 = a1;
        a1 = an;
    }
    return an;
}
