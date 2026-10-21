/*
 * A090016  Permanent of (0,1)-matrix of size n X (n+d) with d=6 and n-1 zeros
 *          not on a line.
 * https://oeis.org/A090016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)*a(n-1) + (n-2)*a(n-2), a(1)=7, a(2)=49
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two defined terms directly. */
    if (n == 0)          /* below the OEIS offset – behaviour not required */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)7;
    if (n == 2)
        return (seq_t)49;

    /* Iterative recurrence – keep only the last two values. */
    seq_t prev2 = (seq_t)7;   /* a(n-2) for the current iteration */
    seq_t prev1 = (seq_t)49;  /* a(n-1) for the current iteration */
    seq_t result;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) = (i+5)*a(i-1) + (i-2)*a(i-2) */
        result = ((seq_t)(i + 5)) * prev1 + ((seq_t)(i - 2)) * prev2;

        /* Shift values for the next iteration */
        prev2 = prev1;
        prev1 = result;
    }
    return prev1;
}
