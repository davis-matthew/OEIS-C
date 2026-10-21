/*
 * A095998  n! * (fractional part of n-th harmonic number).
 * https://oeis.org/A095998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (fractional part of H_n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(n) = (n * a(n-1) + (n-1)!) mod n! */

seq_t a(idx_t n)
{
    /* a(1) = 0, factorial of 1 is 1 */
    seq_t fact = 1;   /* current factorial (k!) while loop runs for k==n   */
    seq_t a_val = 0;   /* a(k-1) during the loop, starts as a(1)=0   */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t prev_fact = fact;          /* (k-1)!                      */
        fact = fact * (seq_t)k;          /* k!                          */
        seq_t temp = a_val * (seq_t)k + prev_fact; /* n*a(n-1)+(n-1)! */

        /* temp < 2*fact, so a(k) = temp modulo fact is just
           temp or temp-fact depending on whether temp >= fact. */
        if (temp >= fact)
            a_val = temp - fact;
        else
            a_val = temp;
    }
    return a_val;
}
