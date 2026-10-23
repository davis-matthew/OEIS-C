/*
 * A103200  A(1)=1, a(2)=2, a(3)=11, a(4)=19; a(n) = a(n-4) + sqrt(60*a(n-2)^2
 *          + 60*a(n-2) + 1) for n >= 5.
 * https://oeis.org/A103200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103200
 * Initial values: a(1)=1, a(2)=2, a(3)=11, a(4)=19
 * Recurrence for n>=5:  a(n) = 8*a(n-2) - a(n-4) + 3
 * (offset 1, so the function returns a(1) for n==1). */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)19;

    /* Prepare previous values:
     * a_am4 = a(i-4), a_am3 = a(i-3), a_am2 = a(i-2), a_am1 = a(i-1) */
    seq_t a_am4 = (seq_t)1,  /* a(1)  */
           a_am3 = (seq_t)2,  /* a(2)  */
           a_am2 = (seq_t)11, /* a(3)  */
           a_am1 = (seq_t)19; /* a(4)  */

    seq_t next = 0;
    for (idx_t i = 5; i <= n; ++i)
    {
        /* Compute a(i) = 8*a(i-2) - a(i-4) + 3 */
        next = (a_am2 << 3) - a_am4 + (seq_t)3;

        /* Shift the window for the next iteration */
        a_am4 = a_am3;
        a_am3 = a_am2;
        a_am2 = a_am1;
        a_am1 = next;
    }
    return next;
}
