/*
 * A028165  Expansion of 1/((1-5*x)*(1-6*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A028165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - 251*a(n-2) + 1066*a(n-3) - 1680*a(n-4)
 *            a(0)=1, a(1)=26, a(2)=425, a(3)=5590 (This is the expansion of
 *            1/((1-5*x)*(1-6*x)*(1-7*x)*(1-8*x)).)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A028165 using the linear recurrence. */
seq_t a(idx_t n)
{
    /* Base cases for n < 4 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)425;
    if (n == 3) return (seq_t)5590;

    /* Use recurrence for n >= 4 */
    seq_t a0 = (seq_t)1;      /* a(n-4)   */
    seq_t a1 = (seq_t)26;     /* a(n-3)   */
    seq_t a2 = (seq_t)425;    /* a(n-2)   */
    seq_t a3 = (seq_t)5590;   /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next =
            (seq_t)26 * a3
          - (seq_t)251 * a2
          + (seq_t)1066 * a1
          - (seq_t)1680 * a0;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
