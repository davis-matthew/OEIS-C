/*
 * A186314  Number of ternary strings of length n which contain 01.
 * https://oeis.org/A186314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + (3^(n-2) - a(n-2)), a(0)=0, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ternary strings of length n which contain the substring "01". */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t a_prev2 = (seq_t)0; /* a(n-2) */
    seq_t a_prev  = (seq_t)0; /* a(n-1) */
    seq_t a_curr;
    seq_t pow3 = (seq_t)1;    /* 3^(i-2), starts with i=2 */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* a(i) = 3*a(i-1) + (3^(i-2) - a(i-2)) */
        a_curr = (seq_t)3 * a_prev + (pow3 - a_prev2);

        /* shift for next iteration */
        a_prev2 = a_prev;
        a_prev  = a_curr;
        pow3   *= (seq_t)3;   /* pow3 = 3^(i-1) for next i+1 */
    }

    return a_prev;
}
