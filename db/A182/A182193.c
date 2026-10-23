/*
 * A182193  Sequence of row differences related to table A182355.
 * https://oeis.org/A182193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 25, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -1, a(1) = 1,
   for n ≥ 2: a(n) = 6*a(n-1) – a(n-2) + 12
   (equivalent to the second recurrence in the OEIS entry). */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)-1;  /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t current;

    /* Compute a(2) … a(n) iteratively */
    for (idx_t i = 2; i <= n; ++i)
    {
        current = (seq_t)(6 * prev1 - prev2 + 12);
        prev2   = prev1;
        prev1   = current;
    }
    return prev1;
}
