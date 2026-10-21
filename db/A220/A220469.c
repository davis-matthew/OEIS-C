/*
 * A220469  Fibonacci 14-step numbers, a(n) = a(n-1) + a(n-2) + ... + a(n-14).
 * https://oeis.org/A220469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+…+a(n-14), with a(0)=0, a(1)=1;
   For n<0 the terms are treated as 0. */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Buffer to keep the last 14 computed terms. */
    seq_t buf[14] = {0};
    buf[0] = 0;
    buf[1] = 1;
    /* sum holds the sum of the last up to 14 terms that are currently
       in buf. Initially it is a(0)+a(1). */
    seq_t sum = buf[0] + buf[1];
    /* idx points to the buffer position that will be overwritten by the
       next term.  It starts at 2, because indices 0 and 1 are already
       occupied. */
    idx_t idx = 2;

    seq_t val;        /* current term a(i) */
    for (idx_t i = 2; i <= n; ++i) {
        /* The recurrence definition gives a(i) as the sum of the
           previous 14 terms (or fewer if i<14). */
        val = sum;

        /* Update the running sum for the next iteration.  Add the new
           term and, when the window exceeds size 14, subtract the
           oldest term that is about to be overwritten. */
        sum += val;
        if (i >= 14) {
            sum -= buf[idx % 14];
        }
        /* Store the new term in the buffer. */
        buf[idx % 14] = val;
        ++idx;

        /* After the loop terminates, val holds a(n). */
    }

    return val;
}
