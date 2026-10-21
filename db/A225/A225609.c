/*
 * A225609  Recurrence a(n) = 2^n*a(n-1) + a(n-2) with a(0)=0, a(1)=1.
 * https://oeis.org/A225609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=2^n*a(n-1)+a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prevprev = (seq_t)0; /* a(n-2) */
    seq_t a_prev     = (seq_t)1; /* a(n-1) */
    seq_t pow2      = (seq_t)2;  /* will become 2^i in the loop */

    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        /* compute 2^i */
        pow2 = pow2 * (seq_t)2;
        /* recurrence */
        cur = a_prev * pow2 + a_prevprev;
        /* shift for next iteration */
        a_prevprev = a_prev;
        a_prev     = cur;
    }
    return a_prev;
}
