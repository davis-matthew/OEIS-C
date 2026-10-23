/*
 * A041320  Numerators of continued fraction convergents to sqrt(174).
 * https://oeis.org/A041320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(174)
 * Recurrence: Let h_(-2)=0, h_(-1)=1.
 * For n>=0, let a_n be the nth partial quotient:
 *   a_0=13, a_1.. repeat (5,4,5,26).
 * Then the numerator is h_n = a_n * h_(n-1) + h_(n-2).
 * This function returns h_n for the given n.                                   */

seq_t a(idx_t n)
{
    /* Partial quotients after the zero index. */
    const unsigned int period[4] = {5u, 4u, 5u, 26u};

    seq_t prev2 = (seq_t)0;   /* h_(n-2)  */
    seq_t prev1 = (seq_t)1;   /* h_(n-1)  */
    seq_t curr   = (seq_t)0;  /* h_n      */
    seq_t coeff  = (seq_t)13; /* a_0       */

    for (idx_t i = (idx_t)0; i <= n; ++i) {
        if (i == (idx_t)0) {
            coeff = (seq_t)13;                /* a_0 */
        } else {
            coeff = (seq_t)period[(i - 1u) % 4u];
        }
        curr = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
