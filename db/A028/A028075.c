/*
 * A028075  Expansion of 1/((1-3x)(1-6x)(1-7x)(1-9x)).
 * https://oeis.org/A028075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 225*a(n-2) + 855*a(n-3) - 1134*a(n-4), with
 *            a(0)=1, a(1)=25, a(2)=400, a(3)=5230. This recurrence follows
 *            from the generating function 1/((1-3x)(1-6x)(1-7x)(1-9x))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)400;
    if (n == 3) return (seq_t)5230;

    /* Initialize the previous four values. */
    seq_t a0 = (seq_t)1;          /* a(n-4) */
    seq_t a1 = (seq_t)25;         /* a(n-3) */
    seq_t a2 = (seq_t)400;        /* a(n-2) */
    seq_t a3 = (seq_t)5230;       /* a(n-1) */
    seq_t next = 0;

    /* Compute a(n) iteratively using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)25 * a3 - (seq_t)225 * a2
               + (seq_t)855 * a1 - (seq_t)1134 * a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = next;
    }

    return next;
}
