/*
 * A019783  Expansion of 1/((1-5*x)*(1-6*x)*(1-8*x)).
 * https://oeis.org/A019783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^(n+2) + 2*5^(n+2) - 3*6^(n+2))/6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=19, a(n)=14*a(n-1)-48*a(n-2)+5^n (n≥2) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t a_prev2 = (seq_t)1;          /* a(0) */
    seq_t a_prev1 = (seq_t)19;         /* a(1) */
    seq_t p5      = (seq_t)5;          /* 5^1   */
    seq_t a_curr  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* 5^i */
        seq_t p5_next = p5 * (seq_t)5;
        seq_t term5   = p5_next;

        /* a(n) = 14*a(n-1) - 48*a(n-2) + 5^n */
        a_curr = (seq_t)14 * a_prev1
                 - (seq_t)48 * a_prev2
                 + term5;

        /* shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
        p5      = p5_next;
    }

    return a_curr;
}
