/*
 * A178222  Partial sums of floor(3^n/4).
 * https://oeis.org/A178222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*3^n - 4*n - 3)/8). Equivalent recurrence:
 *            a(1)=0, a(2)=2, a(n)=a(n-2)+3^(n-1)-1 for n>2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset (a(1) is the first valid term). */
    if (n == 0) return 0;      /* not part of the sequence, harmless return */
    if (n == 1) return 0;      /* a(1) = 0 */
    if (n == 2) return 2;      /* a(2) = 2 */

    /* Initialize for the recurrence */
    seq_t a_prev_prev = 0;     /* a(1) */
    seq_t a_prev      = 2;     /* a(2) */
    seq_t power       = 9;     /* 3^2, needed for n=3 */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_cur = a_prev_prev + power - 1; /* a(i) = a(i-2) + 3^(i-1) - 1 */
        if (i == n) return a_cur;
        a_prev_prev = a_prev;
        a_prev      = a_cur;
        power *= 3;              /* advance 3^(i-1) to 3^i for next iteration */
    }

    /* Should never reach here, but keep a return to satisfy all compilers. */
    return a_prev;
}
