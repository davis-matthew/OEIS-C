/*
 * A041908  Numerators of continued fraction convergents to sqrt(476).
 * https://oeis.org/A041908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=21, a(1)=22, a(n)=p_{n-1}*a_n + p_{n-2}
   where a_n are the partial quotients of sqrt(476):
   a_0=21, and the repeating period is [1,4,2,10,2,4,1,42] */

/* Computes the n-th numerator of convergents to sqrt(476)  (zero‑based). */
seq_t a(idx_t n)
{
    if (n == 0u) /* base case */
        return (seq_t)21;

    /* Partial quotients of sqrt(476) after the initial term a_0=21  */
    static const unsigned int period[8] = {1u, 4u, 2u, 10u, 2u, 4u, 1u, 42u};

    seq_t p_prev = (seq_t)1;    /* p_{-1} */
    seq_t p_curr = (seq_t)21;   /* p_{0}  */
    seq_t p_next;
    idx_t i;

    for (i = 1u; i <= n; ++i) {
        unsigned int a_q = period[(unsigned int)((i - 1u) % 8u)];
        p_next = (seq_t)a_q * p_curr + p_prev;
        p_prev = p_curr;
        p_curr = p_next;
    }
    return p_curr;
}
