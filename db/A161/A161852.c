/*
 * A161852  Solutions to the simultaneous equations m(n)+1=a(n)^2 and
 *          7*m(n)+1=b(n)^2.
 * https://oeis.org/A161852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-2) - a(n-4) with a(1)=1, a(2)=5, a(3)=11, a(4)=79
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle first four terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)79;

    /* iterate forward using the recurrence, keeping only the last four values */
    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)5;   /* a(n-3)   */
    seq_t a2 = (seq_t)11;  /* a(n-2)   */
    seq_t a3 = (seq_t)79;  /* a(n-1)   */
    seq_t new_val;

    for (idx_t k = 5; k <= n; ++k)
    {
        new_val = (seq_t)16 * a2 - a0;  /* a(k) = 16*a(k-2) - a(k-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }
    return a3;
}
