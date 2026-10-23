/*
 * A168642  a(n) = (8*2^n + (-1)^n)/3 for n > 0; a(0) = 1.
 * https://oeis.org/A168642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*2^n + (-1)^n)/3 for n > 0; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) for n > 2 */
/* Initial values: a(0)=1, a(1)=5, a(2)=11 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)11;

    /* Use the recurrence with previous two terms */
    seq_t a_prev2 = (seq_t)5;  /* a(1) */
    seq_t a_prev1 = (seq_t)11; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a_prev1 + (a_prev2 << 1); /* a(n) = a(n-1) + 2*a(n-2) */
        a_prev2 = a_prev1;
        a_prev1 = next;
    }
    return a_prev1;
}
