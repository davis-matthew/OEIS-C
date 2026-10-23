/*
 * A028206  Expansion of 1/((1-6x)(1-7x)(1-9x)(1-12x)).
 * https://oeis.org/A028206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 108*a(n-2) + 7^(n+1) - 6^(n+1); a(0)=1,
 *            a(1)=34
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;

    /* a(n-2) and a(n-1) for the recurrence. */
    seq_t a_prevprev = (seq_t)1;   /* a(0) */
    seq_t a_prev     = (seq_t)34;  /* a(1) */

    /* Current powers of 6 and 7.  
       We start with 6^2 and 7^2 so that at the first iteration
       (i = 2) a multiplication gives 6^(3) and 7^(3), the values
       required for a(2). */
    seq_t pow6 = (seq_t)36;  /* 6^2 */
    seq_t pow7 = (seq_t)49;  /* 7^2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* Advance the powers to 6^(i+1) and 7^(i+1). */
        pow6 *= (seq_t)6;
        pow7 *= (seq_t)7;

        /* Apply the recurrence:
           a(i) = 21*a(i-1) - 108*a(i-2) + 7^(i+1) - 6^(i+1)     */
        seq_t a = (seq_t)21 * a_prev
                - (seq_t)108 * a_prevprev
                + pow7
                - pow6;

        /* Shift for the next iteration. */
        a_prevprev = a_prev;
        a_prev     = a;
    }

    return a_prev;
}
