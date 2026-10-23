/*
 * A027962  Duplicate of A023537.
 * https://oeis.org/A027962
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A027962: Duplicate of A023537.
   Recurrence:
     a(2) = 1
     a(3) = 5
   Let d(k) be the difference a(k+1) - a(k) for k ≥ 1.
     d(1) = 4
     d(2) = 8
     d(k) = d(k-1) + d(k-2) + 3   (k ≥ 3)
   Then a(n) = a(n-1) + d(n-1)   (n ≥ 4) */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;      /* undefined below offset; behaviour ignored */

    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)5;

    seq_t a_val = (seq_t)5;          /* a(3) */
    seq_t d_prevprev = (seq_t)4;     /* d(1) */
    seq_t d_prev     = (seq_t)8;     /* d(2) */

    for (idx_t i = 4; i <= n; ++i)   /* build terms up to a(n) */
    {
        a_val += d_prev;                          /* a(i) = a(i-1) + d(i-1) */

        seq_t d_next = d_prev + d_prevprev + (seq_t)3; /* d(i) = d(i-1)+d(i-2)+3 */
        d_prevprev = d_prev;
        d_prev     = d_next;
    }
    return a_val;
}
