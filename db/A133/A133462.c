/*
 * A133462  a(n+1)-10a(n)=3(-3, -2, -1, 0, 1, 2, 3, 4, 5 ...).
 * https://oeis.org/A133462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133462.
   Recurrence: a(0) = 1,
                a(n+1) = 10*a(n) + 3*(n-3)   for n ≥ 0   */

/* This implementation iteratively applies the recurrence.
   All arithmetic uses only +, -, * (and % if needed).
   Negative terms are handled explicitly to avoid unsigned underflow. */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;        /* a(0) */
    if (n == 0)
        return val;

    for (idx_t i = 0; i < n; ++i) {
        idx_t j = i;               /* j corresponds to the n in the recurrence */
        long long term = 3 * ((long long)j - 3);   /* can be negative for j<3 */

        seq_t temp = val * (seq_t)10;   /* 10*a(j) */

        if (term >= 0)
            temp += (seq_t)term;       /* add positive part */
        else
            temp -= (seq_t)(-term);    /* subtract magnitude of negative part */

        val = temp;
    }

    return val;
}
