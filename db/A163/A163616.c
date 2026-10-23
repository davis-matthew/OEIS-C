/*
 * A163616  a(n) = ((1 + 3*sqrt(2))*(5 + sqrt(2))^n + (1 - 3*sqrt(2))*(5 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2) for n > 1; a(0) = 1, a(1) = 11
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* Iterative computation using the recurrence.
       a0 holds a(n-2), a1 holds a(n-1). */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)11;  /* a(1) */
    seq_t a_next;          /* a(k) for k >= 2 */

    for (idx_t k = 2; k <= n; ++k) {
        /* a(k) = 10*a(k-1) - 23*a(k-2) */
        a_next = (seq_t)(10 * a1) - (seq_t)(23 * a0);
        /* Advance the pair */
        a0 = a1;
        a1 = a_next;
    }
    return a1;
}
