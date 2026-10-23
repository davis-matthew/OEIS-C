/*
 * A038989  Expansion of (1 - x^2 - 2*x^3) / (1 - 2*x - 2*x^2 - 2*x^3 + x^4).
 * https://oeis.org/A038989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) for n>3 Initial
 *            values: a(0)=1, a(1)=2, a(2)=5, a(3)=14
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)14;

    /* Use a sliding window of the last four terms; no dynamic allocation needed */
    seq_t prev4 = (seq_t)1;   /* a(0) */
    seq_t prev3 = (seq_t)2;   /* a(1) */
    seq_t prev2 = (seq_t)5;   /* a(2) */
    seq_t prev1 = (seq_t)14;  /* a(3) */
    seq_t next;               /* a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next term using the recurrence.
           All operations are on type seq_t, so overflow is only possible
           if the result would exceed the capacity of seq_t, which the
           tests guarantee will not happen for the queried values. */
        next = (seq_t)2 * prev1 + (seq_t)2 * prev2 + (seq_t)2 * prev3 - prev4;

        /* Slide the window forward */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;    /* a(n) */
}
