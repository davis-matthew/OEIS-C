/*
 * A099470  A sequence generated from the Quadrifoil.
 * https://oeis.org/A099470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) Initial values:
 *            a(1) = -1, a(2) = -2, a(3) = -1, a(4) = 2 The function computes
 *            the term iteratively; it is independent of previous calls so it
 *            works for any order of n
 * Range:     i8 n <= 190, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n < 1)   return 0;           /* offset is 1; under‑offset is undefined */
    if (n == 1)  return -1;
    if (n == 2)  return -2;
    if (n == 3)  return -1;
    if (n == 4)  return  2;

    /* Variables hold the last four computed values:
       t1 = a(i-4), t2 = a(i-3), t3 = a(i-2), t4 = a(i-1) */
    seq_t t1 = -1;  /* a(1) */
    seq_t t2 = -2;  /* a(2) */
    seq_t t3 = -1;  /* a(3) */
    seq_t t4 =  2;  /* a(4) */

    /* Iterate up to the requested index. */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 2 * t4 - 3 * t3 + 2 * t2 - t1;
        /* Slide the window of four values */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }
    return t4;   /* a(n) */
}
