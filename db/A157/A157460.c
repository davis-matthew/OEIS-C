/*
 * A157460  Expansion of 88*x^2 / (1-483*x+483*x^2-x^3).
 * https://oeis.org/A157460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157460 (OEIS) */
 /*
  * Formula (recurrence):
  *   a(1) = 0
  *   a(2) = 88
  *   a(3) = 483 * a(2)
  *   a(n) = 483 * (a(n-1) - a(n-2)) + a(n-3)   for n > 3
  */

seq_t a(idx_t n)
{
    /* Handle indices below the official offset gracefully */
    if (n < 1)
        return (seq_t)0;

    /* Base cases */
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)88;
    if (n == 3)
        return (seq_t)483 * (seq_t)88;

    /* Iterative computation up to n */
    seq_t c1 = (seq_t)0;            /* a(n-3) */
    seq_t c2 = (seq_t)88;           /* a(n-2) */
    seq_t c3 = (seq_t)483 * (seq_t)88; /* a(n-1) */

    seq_t cn = (seq_t)0;
    for (idx_t i = 4; i <= n; ++i) {
        cn = (seq_t)483 * (c3 - c2) + c1;
        /* rotate window: a(n-3)=a(n-2), a(n-2)=a(n-1), a(n-1)=a(n) */
        c1 = c2;
        c2 = c3;
        c3 = cn;
    }
    return cn;
}
