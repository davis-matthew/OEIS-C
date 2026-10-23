/*
 * A153364  Number of zig-zag paths from top to bottom of a rectangle of width
 *          9 with n rows whose color is not that of the top right corner.
 * https://oeis.org/A153364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-2) - 5*a(n-4), a(1)=4, a(2)=8, a(3)=14, a(4)=28 The
 *            recurrence follows directly from the empirical generating
 *            function x*(4+8*x-6*x^2-12*x^3)/(1-5*x^2+5*x^4)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly.  The comparison is safe
       because n is unsigned and the offsets are small. */
    if (n <= 4) {
        switch ((int)n) {
            case 1: return (seq_t)4;
            case 2: return (seq_t)8;
            case 3: return (seq_t)14;
            case 4: return (seq_t)28;
            default: return (seq_t)0;   /* unreachable, defensive guard */
        }
    }

    /* Previous four terms: a(n-4), a(n-3), a(n-2), a(n-1).  Initialize them
       with the known first four values. */
    seq_t a_nm4 = (seq_t)4;    /* a(1)   */
    seq_t a_nm3 = (seq_t)8;    /* a(2)   */
    seq_t a_nm2 = (seq_t)14;   /* a(3)   */
    seq_t a_nm1 = (seq_t)28;   /* a(4)   */

    seq_t cur = a_nm1;  /* placeholder for the current term */

    /* Iterate from i = 5 up to n, updating the four stored previous terms
       at each step according to the recurrence a(i) = 5*a(i-2) - 5*a(i-4). */
    for (idx_t i = 5; i <= n; ++i) {
        /* Compute a(i) = 5*a(i-2) - 5*a(i-4) */
        cur = (seq_t)5 * a_nm2 - (seq_t)5 * a_nm4;

        /* Shift the window of previous terms for the next iteration. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;  /* cur now holds a(n) */
}
